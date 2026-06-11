// NYL Editor — an ImGui scene editor built on the NYL engine.
//
// Demonstrates the editor-readiness work: a Scene rendered into an off-screen
// framebuffer shown in a viewport panel, a hierarchy, and a registry-driven
// inspector that can add/remove/edit any registered component, plus scene
// save/load. Because ImGui is exported from the engine DLL, the editor links
// the single shared ImGui instance directly.

#include <nyl.h>
#include <imgui.h>

#include <string>

// NOTE: the editor must NOT call OpenGL directly. glad is statically linked into
// both the engine DLL and this executable, so the exe's copy of the glad
// function pointers is never initialised. All GL goes through engine classes
// (RenderSystem, Framebuffer, ...), which live in the DLL and use its glad.

using namespace nyl;

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
        void drawMenuBar();
        void drawHierarchy();
        void drawInspector();
        void drawViewport();
        void buildDefaultScene();

        Scene scene;
        EntityID selected = NullEntity;

        std::unique_ptr<RenderSystem>   renderer;
        std::unique_ptr<ParticleSystem> particles;
        std::unique_ptr<TilemapSystem>  tilemaps;
        std::unique_ptr<FontRenderer>   font;
        Framebuffer fbo;

        glm::vec2 cameraPos{ 0.0f };
        float     zoom = 1.0f;
        bool      playing = false;

        static constexpr int kFboW = 1280;
        static constexpr int kFboH = 720;
    };

    void Editor::buildDefaultScene()
    {
        scene.clear();

        Entity a = scene.createEntity();
        a.addComponent<TransformComponent>(200.0f, 200.0f, 0.0f, 1.0f, 1.0f, 64.0f, 64.0f);

        Entity b = scene.createEntity();
        b.addComponent<TransformComponent>(460.0f, 300.0f, 0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
        ParticleEmitterComponent emitter;
        emitter.startColor = glm::vec4(1.0f, 0.7f, 0.2f, 1.0f);
        emitter.endColor = glm::vec4(1.0f, 0.2f, 0.0f, 0.0f);
        b.addComponent<ParticleEmitterComponent>(emitter);

        Entity c = scene.createEntity();
        c.addComponent<TransformComponent>(760.0f, 160.0f, 0.0f, 1.0f, 1.0f, 90.0f, 50.0f);

        selected = a.id();
    }

    void Editor::Init()
    {
        fbo.create(kFboW, kFboH);
        renderer = std::make_unique<RenderSystem>(static_cast<float>(kFboW), static_cast<float>(kFboH));
        particles = std::make_unique<ParticleSystem>();
        tilemaps = std::make_unique<TilemapSystem>();
        font = std::make_unique<FontRenderer>();
        font->init();

        buildDefaultScene();
        NYL_INFO("NYL Editor ready");
    }

    void Editor::ProcessInput(float deltaTime)
    {
        // pan with arrow keys, zoom with the mouse wheel
        const float panSpeed = 300.0f * deltaTime / (zoom > 0.0f ? zoom : 1.0f);
        if (Input::isKeyDown(GLFW_KEY_LEFT))  cameraPos.x -= panSpeed;
        if (Input::isKeyDown(GLFW_KEY_RIGHT)) cameraPos.x += panSpeed;
        if (Input::isKeyDown(GLFW_KEY_UP))    cameraPos.y -= panSpeed;
        if (Input::isKeyDown(GLFW_KEY_DOWN))  cameraPos.y += panSpeed;

        float scroll = Input::scrollDelta();
        if (scroll != 0.0f)
        {
            zoom += scroll * 0.1f;
            if (zoom < 0.1f) zoom = 0.1f;
            if (zoom > 8.0f) zoom = 8.0f;
        }
    }

    void Editor::Update(float deltaTime)
    {
        if (playing)
            particles->update(scene, deltaTime);
    }

    void Editor::Render(float deltaTime)
    {
        // render the scene into the off-screen framebuffer
        fbo.bind();
        fbo.clear(0.10f, 0.11f, 0.13f, 1.0f);

        renderer->beginFrame(cameraPos, zoom);
        tilemaps->render(scene, *renderer, -10);

        TextureComponent* white = ResourceManager::GetWhiteTexture();
        for (EntityID id : scene.allEntities())
        {
            Entity e(id, &scene);
            auto t = e.getComponent<TransformComponent>();
            if (!t) continue;

            const bool renderable = e.hasComponent<AnimatedComponent>() || e.hasComponent<TextureComponent>();
            if (renderable)
            {
                renderer->drawEntity(e, deltaTime, 0);
            }
            else if (white)
            {
                glm::vec4 col = (id == selected) ? glm::vec4(1.0f, 0.6f, 0.15f, 0.85f)
                                                 : glm::vec4(0.40f, 0.62f, 0.92f, 0.75f);
                renderer->drawSprite(*white, t->position, t->size, t->rotation, col, 0);
            }
        }

        particles->render(scene, *renderer, 5);

        // entity id labels
        for (EntityID id : scene.allEntities())
        {
            Entity e(id, &scene);
            if (auto t = e.getComponent<TransformComponent>())
                font->drawText(*renderer, "E" + std::to_string(id.index),
                               t->position + glm::vec2(0.0f, -12.0f), 1.0f,
                               glm::vec4(1.0f, 1.0f, 1.0f, 0.9f), 1000);
        }

        renderer->endFrame();
        fbo.unbind(m_width, m_height);
    }

    void Editor::OnImGui(float deltaTime)
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        drawMenuBar();
        drawHierarchy();
        drawInspector();
        drawViewport();
    }

    void Editor::drawMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene"))   { buildDefaultScene(); }
                if (ImGui::MenuItem("Save Scene"))  { SceneSerializer::Save(scene, "editor.scene"); }
                if (ImGui::MenuItem("Load Scene"))  { SceneSerializer::Load(scene, "editor.scene"); selected = NullEntity; }
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem(playing ? "Pause" : "Play")) playing = !playing;
            ImGui::EndMainMenuBar();
        }
    }

    void Editor::drawHierarchy()
    {
        ImGui::Begin("Hierarchy");

        if (ImGui::Button("Create Entity"))
        {
            Entity e = scene.createEntity();
            e.addComponent<TransformComponent>(static_cast<float>(kFboW) * 0.5f, static_cast<float>(kFboH) * 0.5f,
                                               0.0f, 1.0f, 1.0f, 48.0f, 48.0f);
            selected = e.id();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete") && scene.isValid(selected))
        {
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
                if (info->inspect)
                    info->inspect(info->getRaw(scene, selected));
                else
                    ImGui::TextDisabled("(no inspector)");
            }
            if (removeRequested) info->remove(scene, selected);
            ImGui::PopID();
        }

        ImGui::Separator();
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("add_component");
        if (ImGui::BeginPopup("add_component"))
        {
            for (const ComponentInfo* info : ComponentRegistry::All())
            {
                if (info->has(scene, selected)) continue;
                if (ImGui::MenuItem(info->name.c_str()))
                    info->addDefault(scene, selected);
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void Editor::drawViewport()
    {
        ImGui::Begin("Viewport");
        ImGui::TextDisabled("Arrows: pan   Wheel: zoom (%.2fx)   %s", zoom, playing ? "PLAYING" : "EDIT");
        ImVec2 avail = ImGui::GetContentRegionAvail();
        // FBO texture is bottom-up; flip V
        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(fbo.colorTexture())),
                     avail, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }
}

nyl::Application* nyl::CreateApplication()
{
    NYL_TRACE("Create NYL Editor");
    return new nyl_editor::Editor(1600, 900, "NYL Editor");
}
