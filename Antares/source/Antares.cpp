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
        // delete collider;
        // delete physics;
        // delete joystick;
        // delete Player;
        // delete debugRenderer;
        // delete Renderer;
        //delete joystick;
        NYL_TRACE("ANTARES destructor");
    }

    void Antares::Init()
    {
        NYL_TRACE("ANTARES init");

        // Load textures
        ResourceManager::LoadTexture("D:/gitHub/nyl/Nyl/resources/chikboy/chikboy_trim.png", true, "chikboy");
        ResourceManager::LoadTexture("D:/gitHub/nyl/Nyl/resources/backgrounds/02.png", true, "background");

        // Configure game objects
        float sizeY = 64.0f;
        float sizeX = 36.0f;
        // float posX = width / 2.0f - sizeX;
        // float posY = height / 2.0f - sizeY;
        point startPoint={60.0f, 280.0f};
        NYL_WARN("Rendering chikboy at x = {0}, y = {1}", startPoint.x, startPoint.y);

        // Create the Player entity and add components
        Player = std::make_shared<Entity>();
        Player->addComponent<PhysicsComponent>(1,1,50);
        Player->addComponent<ColliderComponent>(startPoint.x, startPoint.y, sizeX, sizeY);
        Player->addComponent<TransformComponent>(startPoint.x, startPoint.y, sizeX, sizeY); // Use the new constructor
        // Manually fill collider_platform's fields
        float platformWidth = 400.0f;
        float platformHeight = 50.0f;
        float platformPosX = startPoint.x;
        float platformPosY = startPoint.y + sizeY + 100.0f; // 50 pixels below the player
        
        collider = std::make_shared<ColliderComponent>(startPoint.x, startPoint.y, sizeX, sizeY);
        collider_platform = std::make_shared<ColliderComponent>(0, 350, 190, 90);
        collider_platform_1 = std::make_shared<ColliderComponent>(319, 205, 511-319, 20);
        collider_platform_2 = std::make_shared<ColliderComponent>(platformPosX+500, platformPosY+200, platformWidth, platformHeight);
        groundCollider = std::make_shared<ColliderComponent>(0, height - 5.0f, width, 10.0f);
        // Player = new GameObject(
        //     startPoint.x,
        //     startPoint.y,
        //     sizeX, sizeY,
        //     ResourceManager::GetTexture("chikboy"),
        //     glm::vec3(1.0f), // default color
        //     glm::vec2(0.0f), // default velocity
        //     physics,
        //     collider);
        // Set render specific controls
        //ShaderComponent* spriteShader = ResourceManager::GetShader("sprite");
        Renderer =  new RenderSystem(*ResourceManager::GetShader("sprite"));
            // Set render specific controls
        ShaderComponent* debugShader = ResourceManager::GetShader("debug");
        debugRenderer = new RenderSystem(*debugShader);
        physics = new PhysicsSystem();
        joystick = new Joystick(1);
        if (joystick->isPresent()) {NYL_TRACE("Joystick {0} is present", joystick->getName());}
    }

    void Antares::Update(float deltaTime)
    {
        // Draw background
        Renderer->DrawSprite(*ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);

        // Process input
        ProcessInput(deltaTime);

        // Update collider
        collider->Update(Player->getComponent<TransformComponent>()->position);


        auto colliders = {collider_platform, collider_platform_1, collider_platform_2, groundCollider};

        if(colliderSystem->isColliding(*collider, *groundCollider)){
            NYL_TRACE("Player is colliding with ground");
            HandleCollision(Player, groundCollider);
        }
        for (auto& worldColider : colliders) {
            if (colliderSystem->isColliding(*collider, *worldColider)) {
                HandleCollision(Player, collider);
                break;
            }
        }
        // Apply gravity

        // Draw player
        Renderer->DrawObject(*ResourceManager::GetTexture("chikboy"), Player->getComponent<TransformComponent>()->position, Player->getComponent<TransformComponent>()->size, 0.0f, glm::vec3(1.0f));
        //Player->Draw(*Renderer);

        // Debug draw
        debugRenderer->DrawRectangleOutline(collider->Position, collider->Size, 0.0f, Colors::Green.value);
        debugRenderer->DrawRectangleOutline(collider_platform->Position, collider_platform->Size, 0.0f, Colors::Red.value);
        debugRenderer->DrawRectangleOutline(collider_platform_1->Position, collider_platform_1->Size, 0.0f, Colors::Blue.value);
        debugRenderer->DrawRectangleOutline(collider_platform_2->Position, collider_platform_2->Size, 0.0f, Colors::Blue.value);
        debugRenderer->DrawRectangleOutline(groundCollider->Position, groundCollider->Size, 0.0f, Colors::Red.value);

    }

    void Antares::HandleCollision(std::shared_ptr<Entity> player, std::shared_ptr<ColliderComponent> collider) {
        player->getComponent<TransformComponent>()->position.y = collider->Position.y - player->getComponent<TransformComponent>()->size.y;
        player->getComponent<PhysicsComponent>()->velocity.y = 0; // Stop the player's downward velocity
        player->getComponent<PhysicsComponent>()->canJump = true; // Reset jump state
        //NYL_TRACE("Player is colliding with platform");
    }

    void Antares::ProcessInput(float deltaTime)
    {
        // Define movement and jump speeds
        float speed = 250.0f;
        float jumpSpeed = 400.f;

        // Update joystick state
        joystick->update();

        // Check if joystick is present
        if (!joystick->isPresent()) {
            NYL_TRACE("Joystick not present");
            return;
        }

        // Get joystick inputs
        auto moveX = joystick->axesState(0);
        auto jumpButton = joystick->buttonState(GLFW_JOYSTICK_BTN_DOWN) || joystick->buttonState(GLFW_JOYSTICK_BTN_LEFT);

        // Process horizontal movement
        if (std::abs(moveX) > 0.1)
        {
            Player->getComponent<PhysicsComponent>()->velocity.x = moveX * speed;
            Player->getComponent<PhysicsComponent>()->direction = moveX >= 0 ? 1.0f : -1.0f; // flip the sprite based on the direction
        }
        else{ Player->getComponent<PhysicsComponent>()->velocity.x = 0.0f;}



        // Process jumping
        float groundLevel = height - Player->getComponent<TransformComponent>()->size.y;
        float tolerance = 10.0f;
        if (jumpButton && ( Player->getComponent<PhysicsComponent>()->canJump))
        {
            physics->jump(*Player, jumpSpeed, deltaTime);
            //physics->UpdatePosition(Player, deltaTime, height);
            Player->getComponent<PhysicsComponent>()->canJump = false; // Reset jump state
            NYL_ERROR("foo2");

        }

        physics->applyGravity(*Player, deltaTime);
        physics->updatePhysics(deltaTime, width);
    }
    void Antares::Quit()
    {
        NYL_TRACE("ANTARES quit");
        // delete Renderer;
        // delete Player;
    }
}
Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(1280, 720, "Antares");
}