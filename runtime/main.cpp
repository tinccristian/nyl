// NYL Runtime — a standalone, data-driven player for NYL projects.
//
// Run:  runtime.exe <path-to-.nylproj>   (defaults to the bundled sample)
//
// Loads a project's settings + startup scene and runs it as a real game:
// rendering, fixed-step physics, particle simulation, and a built-in
// platformer controller for the entity whose BoxCollider is flagged "player".
// This is how a project authored in the editor "ships" — no editor, no IDE.

#include <nyl.h>

#include <vector>
#include <cmath>
#include <filesystem>

using namespace nyl;
namespace fs = std::filesystem;

namespace nyl_runtime
{
    class Runtime : public nyl::Application
    {
    public:
        Runtime(int width, int height, const std::string& title)
            : nyl::Application(width, height, title) {}
        ~Runtime() override {}

        void Init() override;
        void ProcessInput(float deltaTime) override;
        void Update(float deltaTime) override {}
        void FixedUpdate(float fixedDeltaTime) override;
        void Render(float deltaTime) override;
        void Quit() override {}

    private:
        void loadProjectAssets();
        void resolveCollision(TransformComponent& t, PhysicsComponent& p,
                              const BoxCollider& c, const CollisionInfo& info);

        Scene scene;
        std::unique_ptr<RenderSystem>   renderer;
        std::unique_ptr<ParticleSystem> particles;
        std::unique_ptr<TilemapSystem>  tilemaps;
        std::unique_ptr<FontRenderer>   font;
        std::unique_ptr<PhysicsSystem>  physics;
        std::unique_ptr<ColliderSystem> collisions;

        Broadphase             worldGrid;
        std::vector<EntityID>  worldColliders;
        EntityID               player = NullEntity;
        bool                   isInAir = false;
    };

    void Runtime::loadProjectAssets()
    {
        if (!Project::IsOpen()) return;
        for (const std::string& file : Project::ListAssets())
        {
            std::string name = fs::path(file).stem().string();
            ResourceManager::LoadTexture(Project::AssetPath(file).c_str(), true, name);
        }
    }

    void Runtime::Init()
    {
        renderer = std::make_unique<RenderSystem>(static_cast<float>(m_width), static_cast<float>(m_height));
        particles = std::make_unique<ParticleSystem>();
        tilemaps = std::make_unique<TilemapSystem>();
        font = std::make_unique<FontRenderer>();
        font->init();
        physics = std::make_unique<PhysicsSystem>();
        collisions = std::make_unique<ColliderSystem>();

        if (Project::IsOpen())
        {
            loadProjectAssets();
            std::string sp = Project::ScenePath(Project::Settings().startupScene);
            if (fs::exists(sp)) SceneSerializer::Load(scene, sp);
        }

        // sync colliders, find the player, and build the static broadphase
        collisions->update(scene);
        worldGrid.clear();
        worldColliders.clear();
        for (EntityID id : scene.allEntities())
        {
            Entity e(id, &scene);
            auto col = e.getComponent<BoxCollider>();
            if (!col) continue;
            if (col->flag == "player" && e.hasComponent<PhysicsComponent>())
            {
                player = id;
            }
            else
            {
                int idx = static_cast<int>(worldColliders.size());
                worldColliders.push_back(id);
                worldGrid.insert(idx, col->min, col->max);
            }
        }
        NYL_INFO("Runtime: scene loaded, {} entities, world colliders={}, player={}",
                 scene.entityCount(), worldColliders.size(),
                 scene.isValid(player) ? std::to_string(player.index) : std::string("none"));
    }

    void Runtime::ProcessInput(float deltaTime)
    {
        if (!scene.isValid(player)) return;
        Entity p(player, &scene);
        auto phys = p.getComponent<PhysicsComponent>();
        auto tr = p.getComponent<TransformComponent>();
        auto anim = p.getComponent<AnimatedComponent>();
        if (!phys || !tr) return;

        const float speed = 220.0f;
        const float jumpSpeed = 480.0f;

        float moveX = 0.0f;
        bool jumpBtn = false;
        if (Input::isKeyDown(GLFW_KEY_A) || Input::isKeyDown(GLFW_KEY_LEFT))  moveX -= 1.0f;
        if (Input::isKeyDown(GLFW_KEY_D) || Input::isKeyDown(GLFW_KEY_RIGHT)) moveX += 1.0f;
        if (Input::isKeyDown(GLFW_KEY_SPACE) || Input::isKeyDown(GLFW_KEY_W) || Input::isKeyDown(GLFW_KEY_UP))
            jumpBtn = true;

        if (std::fabs(moveX) > 0.3f)
        {
            phys->velocity.x = (moveX > 0) ? speed : -speed;
            tr->direction = (moveX > 0) ? 1.0f : -1.0f;
            phys->direction = tr->direction;
            if (anim) anim->SetCurrentAnimation("run");
        }
        else
        {
            phys->velocity.x = 0.0f;
            if (anim) anim->SetCurrentAnimation("idle");
        }

        if (jumpBtn && phys->canJump)
        {
            physics->jump(*phys, jumpSpeed);
            phys->canJump = false;
            isInAir = true;
            phys->velocity.x *= 0.5f;
        }
        else if (!jumpBtn && isInAir)
        {
            physics->applyGravity(*phys, deltaTime);
        }
    }

    void Runtime::resolveCollision(TransformComponent& t, PhysicsComponent& p,
                                   const BoxCollider& c, const CollisionInfo& info)
    {
        switch (info.direction)
        {
        case CollisionDirection::Left:
            t.position.x = c.min.x - t.size.x; p.velocity.x = 0; break;
        case CollisionDirection::Right:
            t.position.x = c.max.x; p.velocity.x = 0; break;
        case CollisionDirection::Bottom:
            t.position.y = c.min.y + t.size.y - 11.0f; p.velocity.y = 0; break;
        case CollisionDirection::Top:
            t.position.y = c.min.y - t.size.y; p.velocity.y = 0; p.canJump = true; isInAir = false; break;
        case CollisionDirection::None:
        default: break;
        }
        t.updateMinMax();
    }

    void Runtime::FixedUpdate(float fixedDeltaTime)
    {
        physics->update(scene, fixedDeltaTime);
        particles->update(scene, fixedDeltaTime);
        collisions->update(scene);

        if (!scene.isValid(player)) return;
        Entity p(player, &scene);
        auto pcol = p.getComponent<BoxCollider>();
        auto ptr = p.getComponent<TransformComponent>();
        auto pph = p.getComponent<PhysicsComponent>();
        if (!pcol || !ptr || !pph) return;

        bool grounded = false;
        for (int idx : worldGrid.query(pcol->min, pcol->max))
        {
            Entity w(worldColliders[idx], &scene);
            auto wc = w.getComponent<BoxCollider>();
            if (!wc) continue;
            auto info = collisions->isColliding(*pcol, *wc);
            if (info.has_value())
            {
                resolveCollision(*ptr, *pph, *wc, info.value());
                if (info->direction == CollisionDirection::Top) grounded = true;
            }
        }
        if (!grounded) pph->canJump = false;
    }

    void Runtime::Render(float deltaTime)
    {
        renderer->beginFrame(glm::vec2(0.0f), 1.0f);
        tilemaps->render(scene, *renderer, -100);

        for (EntityID id : scene.allEntities())
        {
            Entity e(id, &scene);
            if (!e.getComponent<TransformComponent>()) continue;
            if (e.hasComponent<AnimatedComponent>() || e.hasComponent<SpriteComponent>() || e.hasComponent<TextureComponent>())
                renderer->drawEntity(e, deltaTime, 0);
        }

        particles->render(scene, *renderer, 5);
        font->drawText(*renderer, "WASD/Arrows move   Space jump   Esc quit",
                       glm::vec2(12.0f, 12.0f), 2.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.85f), 1000);
        renderer->endFrame();
    }
}

nyl::Application* nyl::CreateApplication()
{
    // project path from the command line, else the bundled sample (exe-relative)
    std::string projPath;
    if (nyl::g_argc > 1)
    {
        projPath = nyl::g_argv[1];
    }
    else
    {
        std::string sample = getExecutableDir() + "/../../projects/sample-platformer/sample-platformer.nylproj";
        if (std::filesystem::exists(sample)) projPath = sample;
    }

    if (!projPath.empty()) nyl::Project::Load(projPath);

    int width = 1280, height = 720;
    std::string title = "NYL Runtime";
    if (nyl::Project::IsOpen())
    {
        const nyl::ProjectSettings& s = nyl::Project::Settings();
        width = s.windowWidth;
        height = s.windowHeight;
        title = s.name;
    }

    NYL_TRACE("Create NYL Runtime");
    return new nyl_runtime::Runtime(width, height, title);
}
