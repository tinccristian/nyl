#include "Antares.h"
#include "core_input.h"
#include <system_renderer.h>
#include "component_collider.h"

using namespace Nyl;
namespace Antares
{
    // RenderSystem* Renderer;
    // RenderSystem* debugRenderer;
    // Joystick* joystick;
    // PhysicsSystem* physics;
    // Entity* Player; // Declare the "Player" variable
    // ColliderComponent* collider;
    // ColliderComponent* collider_platform;
    // ColliderComponent* collider_platform_1;
    // ColliderComponent* collider_platform_2;
    // ColliderComponent* groundCollider;
    std::shared_ptr<Entity> Player; // Declare the "Player" variable
    TransformComponent* transform;
    std::shared_ptr<ColliderComponent> collider;
    std::shared_ptr<ColliderComponent> collider_platform;
    std::shared_ptr<ColliderComponent> collider_platform_1;
    std::shared_ptr<ColliderComponent> collider_platform_2;
    std::shared_ptr<ColliderComponent> groundCollider;
    RenderSystem* Renderer;
    RenderSystem* debugRenderer;
    PhysicsSystem* physics;
    ColliderSystem* colliderSystem;
    Joystick* joystick;

    Antares::Antares(int width, int height, const std::string& title)
        : Nyl::Application(width, height, title)
    {
        Renderer = nullptr;
        debugRenderer = nullptr;
        physics = nullptr;
        colliderSystem = nullptr;
        joystick = nullptr;
        NYL_TRACE("ANTARES constructor");
    }

    Antares::~Antares()
    {
        NYL_TRACE("ANTARES destructor");
    }

    void Antares::Init()
    {
        NYL_TRACE("ANTARES init");

        // Load textures
        ResourceManager::LoadTexture("D:/gitHub/nyl/nyl/resources/backgrounds/background.png", false, "background");
        ResourceManager::LoadTexture("D:/gitHub/nyl/nyl/resources/chikboy/chikboy_trim.png", true, "chikboy");

        // Configure the player
        float sizeY = 64.0f;
        float sizeX = 36.0f;
        point startPoint = {60.0f, 280.0f};

        // Create the Player entity
        Player = std::make_shared<Entity>();
        // Add components to the Player entity

        // (1,1) velocity, 50 mass
        Player->addComponent<PhysicsComponent>(1, 1, 50);
        // add collider component
        Player->addComponent<ColliderComponent>(startPoint.x, startPoint.y, sizeX, sizeY);
        // 
        Player->addComponent<TransformComponent>(startPoint.x, startPoint.y, 0, 1.0f, 1.0f, sizeX, sizeY);
        Player->addComponent<TextureComponent>(*ResourceManager::GetTexture("chikboy"));

        // Create collider components
        float platformWidth = 400.0f;
        float platformHeight = 50.0f;
        float platformPosX = startPoint.x;
        float platformPosY = startPoint.y + sizeY + 100.0f;

        collider = std::make_shared<ColliderComponent>(startPoint.x, startPoint.y, sizeX, sizeY);
        collider_platform = std::make_shared<ColliderComponent>(0, 350, 190, 90);
        collider_platform_1 = std::make_shared<ColliderComponent>(319, 205, 511 - 319, 20);
        collider_platform_2 = std::make_shared<ColliderComponent>(platformPosX + 500, platformPosY + 200, platformWidth, platformHeight);
        groundCollider = std::make_shared<ColliderComponent>(0, height - 5.0f, width, 10.0f);

        // Create systems
        ShaderComponent* spriteShader = ResourceManager::GetShader("sprite");
        Renderer = new RenderSystem(*spriteShader);
        ShaderComponent* debugShader = ResourceManager::GetShader("debug");
        debugRenderer = new RenderSystem(*debugShader);
        physics = new PhysicsSystem();
        // add player entity to physics system
        physics->addEntity(*Player);
        colliderSystem = new ColliderSystem();
        joystick = new Joystick(1);

        if (joystick->isPresent()) {
            NYL_TRACE("Joystick {0} is present", joystick->getName());
        }
    }

    void Antares::Update(float deltaTime)
    {
        // Draw background
        TextureComponent* background = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(*background, glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height));

        // Process input
        ProcessInput(deltaTime);

        // Update collider
        collider->Update(Player->getComponent<TransformComponent>()->position);

        auto colliders = {collider_platform, collider_platform_1, collider_platform_2, groundCollider};

        // Check collision with platforms
        bool isCollidingWithPlatform = false;
        for (auto& worldCollider : colliders) {
            if (colliderSystem->isColliding(*collider, *worldCollider)) {
                HandleCollision(Player, worldCollider);
                isCollidingWithPlatform = true;
                break;
            }
        }
        //Draw player
        //Renderer->DrawSprite(*ResourceManager::GetTexture("chikboy"), Player->getComponent<TransformComponent>()->position, Player->getComponent<TransformComponent>()->size, 0.0f, glm::vec3(1.0f));
        Renderer->DrawEntity(*Player);
        // Debug draw
        debugRenderer->DrawRectangleOutline(collider->Position, collider->Size, 0.0f, Colors::Green.value);
        debugRenderer->DrawRectangleOutline(collider_platform->Position, collider_platform->Size, 0.0f, Colors::Red.value);
        debugRenderer->DrawRectangleOutline(collider_platform_1->Position, collider_platform_1->Size, 0.0f, Colors::Blue.value);
        debugRenderer->DrawRectangleOutline(collider_platform_2->Position, collider_platform_2->Size, 0.0f, Colors::Blue.value);
        debugRenderer->DrawRectangleOutline(groundCollider->Position, groundCollider->Size, 0.0f, Colors::Red.value);
    }

    void Antares::HandleCollision(std::shared_ptr<Entity> player, std::shared_ptr<ColliderComponent> collider)
    {
        //float offset = 30.0f; // Adjust this value as needed
        player->getComponent<TransformComponent>()->position.y = collider->Position.y - player->getComponent<TransformComponent>()->size.y;// - offset;
        player->getComponent<PhysicsComponent>()->velocity.y = 0;
        player->getComponent<PhysicsComponent>()->canJump = true;
    }

    void Antares::ProcessInput(float deltaTime)
    {
        float speed = 250.0f;
        float jumpSpeed = 600.0f;

        joystick->update();

        if (!joystick->isPresent()) {
            NYL_TRACE("Joystick not present");
            return;
        }

        auto moveX = joystick->axesState(0);
        auto jumpButton = joystick->buttonState(GLFW_JOYSTICK_BTN_DOWN) || joystick->buttonState(GLFW_JOYSTICK_BTN_LEFT);

        if (std::abs(moveX) > 0.1) {
            Player->getComponent<PhysicsComponent>()->velocity.x = moveX * speed;
            Player->getComponent<PhysicsComponent>()->direction = moveX >= 0 ? 1.0f : -1.0f;
            Player->getComponent<TransformComponent>()->direction = moveX >= 0 ? 1.0f : -1.0f;
        }
        else {
            Player->getComponent<PhysicsComponent>()->velocity.x = 0.0f;
        }

        float groundLevel = height - Player->getComponent<TransformComponent>()->size.y;
        float tolerance = 10.0f;

        if (jumpButton && Player->getComponent<PhysicsComponent>()->canJump) {
            NYL_TRACE("Jump!");
            physics->jump(*Player, jumpSpeed, deltaTime);
            Player->getComponent<PhysicsComponent>()->canJump = false;
        }

        physics->applyGravity(*Player, deltaTime);
        physics->updatePhysics(deltaTime, width);
    }

    void Antares::Quit()
    {
        NYL_TRACE("ANTARES quit");
    }
}

Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(1280, 640, "Antares");
    //return new Antares::Antares(1920, 1080, "Antares");
}