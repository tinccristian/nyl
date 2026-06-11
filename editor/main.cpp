// NYL Editor — an ImGui scene + project editor built on the NYL engine.
//
// Create / open / configure projects, author scenes, manage assets, and run a
// play preview — without an external IDE. ImGui is exported from the engine DLL
// so the editor links the single shared ImGui instance.
//
// NOTE: the editor must NOT call OpenGL directly. glad is statically linked into
// both the engine DLL and this executable, so the exe's copy of the glad
// function pointers is never initialised. All GL goes through engine classes
// (RenderSystem, Framebuffer, ...), which live in the DLL and use its glad.

#include <nyl.h>
#include <imgui.h>

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstdlib>

using namespace nyl;
namespace fs = std::filesystem;

namespace nyl_editor
{
    class Editor : public nyl::Application
    {
    public:
        Editor(int width, int height, const std::string& title)
            : nyl::Application(width, height, title) {}
        ~Editor() override {}

        void Init() override;
        void ProcessInput(float deltaTime) override;
        void Update(float deltaTime) override;
        void Render(float deltaTime) override;
        void OnImGui(float deltaTime) override;
        void Quit() override {}

    private:
        // panels
        void drawMenuBar();
        void drawHierarchy();
        void drawInspector();
        void drawAssets();
        void drawProjectPanel();
        void drawViewport();

        // project / scene operations
        void newProjectDialog();
        void openProjectDialog();
        void openProject(const std::string& projectFile);
        void loadProjectAssets();
        void newScene();
        void openSceneDialog();
        void saveScene();
        void saveSceneAs();
        void loadScene(const std::string& path);
        void setStartupScene();
        void runProject();
        void refreshTitle();

        // undo/redo (scene snapshots)
        void pushUndo();
        void undo();
        void redo();

        // last-project memory
        std::string configPath() const { return "editor_config.txt"; }
        void saveConfig();
        std::string loadConfig();

        // viewport <-> world helpers
        glm::vec2 viewportToWorld(float nx, float ny) const;
        EntityID  pickEntity(glm::vec2 world) const;

        Scene scene;
        EntityID selected = NullEntity;
        std::string currentScenePath;

        std::unique_ptr<RenderSystem>   renderer;
        std::unique_ptr<ParticleSystem> particles;
        std::unique_ptr<TilemapSystem>  tilemaps;
        std::unique_ptr<FontRenderer>   font;
        Framebuffer fbo;

        glm::vec2 cameraPos{ 0.0f };
        float     zoom = 1.0f;
        bool      playing = false;

        bool      dragging = false;
        bool      dragUndoPushed = false;
        glm::vec2 dragOffset{ 0.0f };

        std::vector<std::string> undoStack;
        std::vector<std::string> redoStack;

        static constexpr int kFboW = 1280;
        static constexpr int kFboH = 720;
    };

    // ------------------------------------------------------------------ setup

    void Editor::Init()
    {
        fbo.create(kFboW, kFboH);
        renderer = std::make_unique<RenderSystem>(static_cast<float>(kFboW), static_cast<float>(kFboH));
        particles = std::make_unique<ParticleSystem>();
        tilemaps = std::make_unique<TilemapSystem>();
        font = std::make_unique<FontRenderer>();
        font->init();

        // open the last project, else the bundled sample, else start empty
        std::string last = loadConfig();
        if (!last.empty() && fs::exists(last))
        {
            openProject(last);
        }
        else
        {
            // bundled sample, resolved relative to the executable (bin/<cfg>/)
            for (const std::string& cand : {
                     getExecutableDir() + "/../../projects/sample-platformer/sample-platformer.nylproj",
                     std::string("projects/sample-platformer/sample-platformer.nylproj") })
            {
                if (fs::exists(cand)) { openProject(cand); break; }
            }
        }
        refreshTitle();
        NYL_INFO("NYL Editor ready");
    }

    void Editor::ProcessInput(float deltaTime)
    {
        const float panSpeed = 300.0f * deltaTime / (zoom > 0.0f ? zoom : 1.0f);
        if (Input::isKeyDown(GLFW_KEY_LEFT))  cameraPos.x -= panSpeed;
        if (Input::isKeyDown(GLFW_KEY_RIGHT)) cameraPos.x += panSpeed;
        if (Input::isKeyDown(GLFW_KEY_UP))    cameraPos.y -= panSpeed;
        if (Input::isKeyDown(GLFW_KEY_DOWN))  cameraPos.y += panSpeed;

        float scroll = Input::scrollDelta();
        if (scroll != 0.0f)
        {
            zoom += scroll * 0.1f;
            zoom = (zoom < 0.1f) ? 0.1f : (zoom > 8.0f ? 8.0f : zoom);
        }
    }

    void Editor::Update(float deltaTime)
    {
        if (playing)
            particles->update(scene, deltaTime);
    }

    void Editor::Render(float deltaTime)
    {
        fbo.bind();
        fbo.clear(0.10f, 0.11f, 0.13f, 1.0f);

        renderer->beginFrame(cameraPos, zoom);
        tilemaps->render(scene, *renderer, -100);

        TextureComponent* white = ResourceManager::GetWhiteTexture();
        for (EntityID id : scene.allEntities())
        {
            Entity e(id, &scene);
            auto t = e.getComponent<TransformComponent>();
            if (!t) continue;

            if (id == selected && white)
            {
                glm::vec2 pad(4.0f);
                renderer->drawSprite(*white, t->position - pad, t->size + pad * 2.0f, t->rotation,
                                     glm::vec4(1.0f, 0.6f, 0.15f, 0.5f), 1000);
            }

            const bool renderable = e.hasComponent<AnimatedComponent>()
                                 || e.hasComponent<SpriteComponent>()
                                 || e.hasComponent<TextureComponent>();
            if (renderable)
                renderer->drawEntity(e, deltaTime, 0);
            else if (white)
                renderer->drawSprite(*white, t->position, t->size, t->rotation,
                                     glm::vec4(0.40f, 0.62f, 0.92f, 0.6f), 0);
        }

        particles->render(scene, *renderer, 5);
        renderer->endFrame();
        fbo.unbind(m_width, m_height);
    }

    void Editor::OnImGui(float deltaTime)
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // keyboard shortcuts
        ImGuiIO& io = ImGui::GetIO();
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z)) undo();
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y)) redo();
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S)) saveScene();
        if (ImGui::IsKeyPressed(ImGuiKey_F5)) runProject();

        drawMenuBar();
        drawProjectPanel();
        drawHierarchy();
        drawInspector();
        drawAssets();
        drawViewport();
    }

    // ------------------------------------------------------------- project ops

    void Editor::saveConfig()
    {
        std::ofstream o(configPath());
        if (Project::IsOpen()) o << Project::ProjectFile();
    }

    std::string Editor::loadConfig()
    {
        std::ifstream i(configPath());
        std::string p;
        std::getline(i, p);
        return p;
    }

    void Editor::loadProjectAssets()
    {
        if (!Project::IsOpen()) return;
        for (const std::string& file : Project::ListAssets())
        {
            std::string path = Project::AssetPath(file);
            std::string name = fs::path(file).stem().string();
            ResourceManager::LoadTexture(path.c_str(), true, name);
        }
    }

    void Editor::openProject(const std::string& projectFile)
    {
        if (!Project::Load(projectFile)) return;
        loadProjectAssets();

        scene.clear();
        selected = NullEntity;
        currentScenePath.clear();

        const ProjectSettings& s = Project::Settings();
        if (!s.startupScene.empty())
        {
            std::string sp = Project::ScenePath(s.startupScene);
            if (fs::exists(sp)) loadScene(sp);
        }
        saveConfig();
        refreshTitle();
    }

    void Editor::newProjectDialog()
    {
        std::string folder = FileDialog::SelectFolder("Choose an (empty) folder for the new project");
        if (folder.empty()) return;
        std::string name = fs::path(folder).filename().string();
        if (name.empty()) name = "Untitled";
        if (Project::Create(folder, name))
            openProject(Project::ProjectFile());
    }

    void Editor::openProjectDialog()
    {
        std::string f = FileDialog::OpenFile("Open NYL project", { "*.nylproj" }, "NYL projects");
        if (!f.empty()) openProject(f);
    }

    void Editor::loadScene(const std::string& path)
    {
        if (SceneSerializer::Load(scene, path))
        {
            currentScenePath = path;
            selected = NullEntity;
            refreshTitle();
        }
    }

    void Editor::newScene()
    {
        scene.clear();
        selected = NullEntity;
        currentScenePath.clear();
        refreshTitle();
    }

    void Editor::openSceneDialog()
    {
        std::string f = FileDialog::OpenFile("Open scene", { "*.scene" }, "NYL scenes");
        if (!f.empty()) loadScene(f);
    }

    void Editor::saveScene()
    {
        if (currentScenePath.empty()) saveSceneAs();
        else SceneSerializer::Save(scene, currentScenePath);
    }

    void Editor::saveSceneAs()
    {
        std::string def = Project::IsOpen() ? Project::ScenePath("untitled.scene") : "untitled.scene";
        std::string f = FileDialog::SaveFile("Save scene", def, { "*.scene" }, "NYL scenes");
        if (!f.empty())
        {
            SceneSerializer::Save(scene, f);
            currentScenePath = f;
            refreshTitle();
        }
    }

    void Editor::setStartupScene()
    {
        if (Project::IsOpen() && !currentScenePath.empty())
        {
            Project::Settings().startupScene = fs::path(currentScenePath).filename().string();
            Project::Save();
        }
    }

    void Editor::refreshTitle()
    {
        std::string t = "NYL Editor";
        if (Project::IsOpen()) t += "  -  " + Project::Settings().name;
        if (!currentScenePath.empty()) t += "  -  " + fs::path(currentScenePath).filename().string();
        setWindowTitle(t);
    }

    void Editor::runProject()
    {
        if (!Project::IsOpen()) return;
        // persist the latest scene + settings so the runtime loads them
        if (!currentScenePath.empty()) SceneSerializer::Save(scene, currentScenePath);
        Project::Save();

        std::string exe = getExecutableDir() + "/runtime.exe";
        // `start` launches the runtime detached so the editor keeps running
        std::string cmd = "start \"NYL Runtime\" \"" + exe + "\" \"" + Project::ProjectFile() + "\"";
        std::system(cmd.c_str());
        NYL_INFO("Launched runtime for {}", Project::ProjectFile());
    }

    void Editor::pushUndo()
    {
        undoStack.push_back(SceneSerializer::ToJsonString(scene));
        if (undoStack.size() > 64) undoStack.erase(undoStack.begin());
        redoStack.clear();
    }

    void Editor::undo()
    {
        if (undoStack.empty()) return;
        redoStack.push_back(SceneSerializer::ToJsonString(scene));
        SceneSerializer::FromJsonString(scene, undoStack.back());
        undoStack.pop_back();
        selected = NullEntity;
    }

    void Editor::redo()
    {
        if (redoStack.empty()) return;
        undoStack.push_back(SceneSerializer::ToJsonString(scene));
        SceneSerializer::FromJsonString(scene, redoStack.back());
        redoStack.pop_back();
        selected = NullEntity;
    }

    // ------------------------------------------------------------------ panels

    void Editor::drawMenuBar()
    {
        if (!ImGui::BeginMainMenuBar()) return;

        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("New Project...")) newProjectDialog();
            if (ImGui::MenuItem("Open Project...")) openProjectDialog();
            if (ImGui::MenuItem("Save Project", nullptr, false, Project::IsOpen())) Project::Save();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("New Scene")) newScene();
            if (ImGui::MenuItem("Open Scene...")) openSceneDialog();
            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) saveScene();
            if (ImGui::MenuItem("Save Scene As...")) saveSceneAs();
            ImGui::Separator();
            if (ImGui::MenuItem("Set as Startup Scene", nullptr, false,
                                Project::IsOpen() && !currentScenePath.empty()))
                setStartupScene();
            ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::MenuItem(playing ? "Pause" : "Play")) playing = !playing;
        if (ImGui::MenuItem("Run Project", "F5", false, Project::IsOpen())) runProject();

        ImGui::EndMainMenuBar();
    }

    void Editor::drawProjectPanel()
    {
        ImGui::Begin("Project");

        if (!Project::IsOpen())
        {
            ImGui::TextDisabled("No project open.");
            if (ImGui::Button("New Project...")) newProjectDialog();
            ImGui::SameLine();
            if (ImGui::Button("Open Project...")) openProjectDialog();
            ImGui::End();
            return;
        }

        ProjectSettings& s = Project::Settings();

        char nameBuf[128];
        std::snprintf(nameBuf, sizeof(nameBuf), "%s", s.name.c_str());
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) { s.name = nameBuf; refreshTitle(); }

        ImGui::DragInt("Window Width", &s.windowWidth, 1.0f, 320, 4096);
        ImGui::DragInt("Window Height", &s.windowHeight, 1.0f, 240, 4096);

        const char* startupPreview = s.startupScene.empty() ? "(none)" : s.startupScene.c_str();
        if (ImGui::BeginCombo("Startup Scene", startupPreview))
        {
            for (const std::string& sc : Project::ListScenes())
                if (ImGui::Selectable(sc.c_str(), sc == s.startupScene)) s.startupScene = sc;
            ImGui::EndCombo();
        }

        if (ImGui::Button("Save Project Settings")) Project::Save();

        ImGui::Separator();
        ImGui::TextWrapped("Root: %s", Project::RootDir().c_str());
        ImGui::TextWrapped("Scene: %s", currentScenePath.empty() ? "(unsaved)" : currentScenePath.c_str());
        ImGui::End();
    }

    void Editor::drawHierarchy()
    {
        ImGui::Begin("Hierarchy");

        if (ImGui::Button("Entity"))
        {
            pushUndo();
            Entity e = scene.createEntity();
            e.addComponent<TransformComponent>(static_cast<float>(kFboW) * 0.5f, static_cast<float>(kFboH) * 0.5f,
                                               0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
            selected = e.id();
        }
        ImGui::SameLine();
        if (ImGui::Button("Sprite"))
        {
            pushUndo();
            Entity e = scene.createEntity();
            e.addComponent<TransformComponent>(static_cast<float>(kFboW) * 0.5f, static_cast<float>(kFboH) * 0.5f,
                                               0.0f, 1.0f, 1.0f, 64.0f, 64.0f);
            auto& sp = e.addComponent<SpriteComponent>();
            auto names = ResourceManager::TextureNames();
            if (!names.empty()) sp.texture = names.front();
            selected = e.id();
        }
        ImGui::SameLine();
        if (ImGui::Button("Duplicate") && scene.isValid(selected))
        {
            pushUndo();
            EntityID dup = scene.createEntityID();
            ComponentRegistry::CloneEntity(scene, selected, dup);
            selected = dup;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete") && scene.isValid(selected))
        {
            pushUndo();
            scene.destroyEntity(selected);
            selected = NullEntity;
        }
        ImGui::Separator();

        for (EntityID id : scene.allEntities())
        {
            std::string label = "Entity " + std::to_string(id.index);
            if (ImGui::Selectable(label.c_str(), id == selected))
                selected = id;
        }

        ImGui::Text("Entities: %zu", scene.entityCount());
        ImGui::End();
    }

    void Editor::drawInspector()
    {
        ImGui::Begin("Inspector");

        if (!scene.isValid(selected))
        {
            ImGui::TextDisabled("No entity selected");
            ImGui::End();
            return;
        }

        ImGui::Text("Entity %u", selected.index);
        ImGui::Separator();

        for (const ComponentInfo* info : ComponentRegistry::All())
        {
            if (!info->has(scene, selected)) continue;
            ImGui::PushID(info->name.c_str());
            bool open = ImGui::CollapsingHeader(info->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
            bool removeRequested = false;
            if (ImGui::BeginPopupContextItem("ctx"))
            {
                if (ImGui::MenuItem("Remove Component")) removeRequested = true;
                ImGui::EndPopup();
            }
            if (open)
            {
                if (info->inspect) info->inspect(info->getRaw(scene, selected));
                else ImGui::TextDisabled("(no inspector)");
            }
            if (removeRequested) { pushUndo(); info->remove(scene, selected); }
            ImGui::PopID();
        }

        ImGui::Separator();
        if (ImGui::Button("Add Component")) ImGui::OpenPopup("add_component");
        if (ImGui::BeginPopup("add_component"))
        {
            for (const ComponentInfo* info : ComponentRegistry::All())
            {
                if (info->has(scene, selected)) continue;
                if (ImGui::MenuItem(info->name.c_str())) { pushUndo(); info->addDefault(scene, selected); }
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void Editor::drawAssets()
    {
        ImGui::Begin("Assets");

        if (Project::IsOpen())
        {
            if (ImGui::Button("Import Texture..."))
            {
                std::string f = FileDialog::OpenFile("Import texture",
                                                     { "*.png", "*.jpg", "*.jpeg", "*.bmp" }, "Images");
                if (!f.empty())
                {
                    std::error_code ec;
                    std::string dest = Project::AssetPath(fs::path(f).filename().string());
                    fs::copy_file(f, dest, fs::copy_options::overwrite_existing, ec);
                    std::string name = fs::path(f).stem().string();
                    ResourceManager::LoadTexture(dest.c_str(), true, name);
                }
            }
            ImGui::Separator();
            ImGui::TextDisabled("Project assets:");
            for (const std::string& a : Project::ListAssets())
                ImGui::BulletText("%s", a.c_str());
            ImGui::Separator();
        }

        ImGui::TextDisabled("Loaded textures:");
        for (const std::string& name : ResourceManager::TextureNames())
            ImGui::BulletText("%s", name.c_str());

        ImGui::End();
    }

    glm::vec2 Editor::viewportToWorld(float nx, float ny) const
    {
        const glm::vec2 view(nx * kFboW, ny * kFboH);
        const glm::vec2 c(kFboW * 0.5f, kFboH * 0.5f);
        const float z = (zoom > 0.0f) ? zoom : 1.0f;
        return (view - c) / z + c + cameraPos;
    }

    EntityID Editor::pickEntity(glm::vec2 world) const
    {
        EntityID hit = NullEntity;
        for (EntityID id : scene.allEntities())
        {
            Entity e(id, const_cast<Scene*>(&scene));
            auto t = e.getComponent<TransformComponent>();
            if (!t) continue;
            glm::vec2 lo = t->position;
            glm::vec2 hi = t->position + t->size;
            if (world.x >= lo.x && world.x <= hi.x && world.y >= lo.y && world.y <= hi.y)
                hit = id;
        }
        return hit;
    }

    void Editor::drawViewport()
    {
        ImGui::Begin("Viewport");
        ImGui::TextDisabled("Click: select   Drag: move   Arrows: pan   Wheel: zoom (%.2fx)   %s",
                            zoom, playing ? "PLAYING" : "EDIT");

        ImVec2 avail = ImGui::GetContentRegionAvail();
        ImVec2 imgMin = ImGui::GetCursorScreenPos();
        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(fbo.colorTexture())),
                     avail, ImVec2(0, 1), ImVec2(1, 0));

        if (avail.x > 1.0f && avail.y > 1.0f && ImGui::IsItemHovered())
        {
            ImVec2 mouse = ImGui::GetMousePos();
            float nx = (mouse.x - imgMin.x) / avail.x;
            float ny = (mouse.y - imgMin.y) / avail.y;
            glm::vec2 world = viewportToWorld(nx, ny);

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                selected = pickEntity(world);
                dragging = scene.isValid(selected);
                if (dragging)
                {
                    Entity e(selected, &scene);
                    if (auto t = e.getComponent<TransformComponent>())
                        dragOffset = world - t->position;
                }
            }
            if (dragging && ImGui::IsMouseDragging(ImGuiMouseButton_Left) && scene.isValid(selected))
            {
                if (!dragUndoPushed) { pushUndo(); dragUndoPushed = true; }
                Entity e(selected, &scene);
                if (auto t = e.getComponent<TransformComponent>())
                {
                    t->position = world - dragOffset;
                    t->updateMinMax();
                }
            }
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) { dragging = false; dragUndoPushed = false; }

        ImGui::End();
    }
}

nyl::Application* nyl::CreateApplication()
{
    NYL_TRACE("Create NYL Editor");
    return new nyl_editor::Editor(1600, 900, "NYL Editor");
}
