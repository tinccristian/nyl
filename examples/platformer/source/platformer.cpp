#include "platformer.h"
#include "core_input.h"
#include <system_renderer.h>
#include "component_collider.h"
#include "component_camera.h"
#include "system_camera.h"

using namespace Nyl;
namespace platformer
{

    std::shared_ptr<Entity> Player; 
    TransformComponent* transform;
    std::vector<std::shared_ptr<BoxCollider>> colliders;
    std::shared_ptr<Camera> camera;
    std::unique_ptr<CameraSystem> cameraManager;
    std::unique_ptr<RenderSystem> Renderer;
    std::unique_ptr<RenderSystem> debugRenderer;
    std::unique_ptr<PhysicsSystem> physics;
    std::unique_ptr<ColliderSystem> collisions;
    std::unique_ptr<Joystick> joystick;
    float cloudTimer = 0.0f;
    const float cloudInterval = 5.0f;

    platformer::platformer(int width, int height, const std::string& title)
        : Nyl::Application(width, height, title)
    {
        //NYL_TRACE("ANTARES constructor");
    }

    platformer::~platformer()
    {
        //NYL_TRACE("ANTARES destructor");
    }

    void platformer::Init()
    {
        // Load resources
        LoadResources();

        // Configure the player
        ConfigurePlayer();

        // Create collider components
        CreateColliders();

        // Create systems
        CreateSystems();
    }

    void platformer::Update(float deltaTime)
    {
        physics->updatePhysics(deltaTime, width);

        // Update camera
        cameraManager->update(*Player);
        
        // Draw background
        TextureComponent* background = ResourceManager::GetTexture("lv2");
        Renderer->DrawSprite(*background, glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height));

        // Draw clouds
        TextureComponent* cloud = ResourceManager::GetTexture("cloud");
        Renderer->DrawSprite(*cloud, glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f));
        Renderer->DrawSprite(*cloud, glm::vec2(250.0f, 15.0f), glm::vec2(100.0f, 100.0f));
        Renderer->DrawSprite(*cloud, glm::vec2(454.0f,32.0f), glm::vec2(100.0f, 100.0f));
        Renderer->DrawSprite(*cloud, glm::vec2(790.0f,80.0f), glm::vec2(100.0f, 100.0f));
        Renderer->DrawSprite(*cloud, glm::vec2(1100.0f, 150.0f), glm::vec2(100.0f, 100.0f));
        // Process input
        ProcessInput(deltaTime);

        // Update collider
        collisions->update();

        // Check collision with platforms
        bool isCollidingWithPlatform = false;
        for (auto& worldCollider : colliders) {
            bool wasColliding = worldCollider->isColliding;
            worldCollider->isColliding = collisions->isColliding(*Player->getComponent<BoxCollider>(), *worldCollider);

            if (worldCollider->isColliding) {
                // if (!wasColliding) {
                //     debugRenderer->DrawRectangleOutline(worldCollider->getPosition(), worldCollider->getSize(), 0.0f, Colors::Red.value);
                // }
                HandleCollision(Player, worldCollider);
                isCollidingWithPlatform = true;
                break;
            }// else if (wasColliding) {
                //debugRenderer->DrawRectangleOutline(worldCollider->getPosition(), worldCollider->getSize(), 0.0f, Colors::Green.value);
            //}
        }
        if (!isCollidingWithPlatform) {
            Player->getComponent<PhysicsComponent>()->canJump = false;
        }
        //Draw player
        Renderer->DrawEntity(*Player);
        // Debug draw
        //debugRenderer->DrawRectangleOutline(Player->getComponent<BoxCollider>()->getPosition(), Player->getComponent<BoxCollider>()->getSize(), 0.0f, Colors::Green.value);
    }

#pragma region init_helper_foo
    void platformer::LoadResources()
    {
        std::string resourcePath = getFullPath("../../../engine/resources/");

        std::vector<std::string> texturePaths = {
            resourcePath + "backgrounds/lv2.png",
            resourcePath + "characters/chikboy_trim.png",
            resourcePath + "backgrounds/cloud.png"
        };

        for (const std::string& path : texturePaths) {
            std::string textureName = path.substr(path.find_last_of('/') + 1);
            textureName = textureName.substr(0, textureName.find_last_of('.'));
            if (!ResourceManager::LoadTexture(path.c_str(), true, textureName)) {
                NYL_ERROR("Failed to load texture: {}", textureName);
                return;
            }
        }
    }
    void platformer::ConfigurePlayer()
    {
        camera = std::make_shared<Camera>(0, 0, 800, 600,0.5f);
        // Configure the player
        float sizeY = 64.0f;
        float sizeX = 36.0f;
        point startPoint = {0.0f, 0.0f};

        // Create the Player entity
        Player = std::make_shared<Entity>();

        Player->addComponent<Camera>(*camera);
        // (1,1) velocity, 50 mass
        Player->addComponent<PhysicsComponent>(1, 2, 50);
        // add collider component
        Player->addComponent<TransformComponent>(startPoint.x, startPoint.y, 0, 1.0f, 1.0f, sizeX, sizeY);
        auto transform = Player->getComponent<TransformComponent>();
        Player->addComponent<BoxCollider>(transform->min, transform->max),"player";
        Player->addComponent<TextureComponent>(*ResourceManager::GetTexture("chikboy_trim"));
    }
    void platformer::CreateColliders()
    {
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(0,height), glm::vec2(width,height-5.0f), "ground"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(0,384),   glm::vec2(364,335),   "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(544,714), glm::vec2(798,458), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(309,202), glm::vec2(358,153), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(163,80),  glm::vec2(212,31) , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(429,524), glm::vec2(478,475), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(291,614), glm::vec2(340,565), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(542,282), glm::vec2(796,246), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(615,245), glm::vec2(664,5)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(872,524), glm::vec2(922,475)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(1077,650), glm::vec2(1126,601)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(943,384), glm::vec2(1274,335)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(945,192), glm::vec2(994,143)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(1169,66), glm::vec2(1218,17)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(712,84), glm::vec2(754,5)  , "platform"));
    }
    void platformer::CreateSystems()
    {
            ShaderComponent* spriteShader = ResourceManager::GetShader("sprite");
            Renderer = std::make_unique<RenderSystem>(*spriteShader,this->width,this->height);

            ShaderComponent* debugShader = ResourceManager::GetShader("debug");
            debugRenderer = std::make_unique<RenderSystem>(*debugShader,this->width,this->height);

            cameraManager = std::make_unique<CameraSystem>(*camera);
            physics = std::make_unique<PhysicsSystem>();
            // add player entity to physics system
            physics->addEntity(*Player);
            collisions = std::make_unique<ColliderSystem>();
            collisions->addEntity(*Player);
            joystick = std::make_unique<Joystick>(1);

            if (joystick->isPresent()) {
                NYL_TRACE("Joystick {0} is connected.", joystick->getName());
            }
            else {
                NYL_TRACE("Joystick not connected");
            }
    }
#pragma endregion
    void platformer::HandleCollision(std::shared_ptr<Entity> player, std::shared_ptr<BoxCollider> collider)
{
    //float offset = 0.0f; // Adjust this value as needed
    player->getComponent<TransformComponent>()->position.y = collider->getPosition().y - player->getComponent<TransformComponent>()->size.y;// - offset;
    player->getComponent<PhysicsComponent>()->velocity.y = 0;
    player->getComponent<PhysicsComponent>()->canJump = true;

    // Check if player is on top of the platform
    if (player->getComponent<TransformComponent>()->position.y >= collider->getPosition().y + collider->getSize().y) {
        player->getComponent<PhysicsComponent>()->canJump = true;
    }

}

    void platformer::ProcessInput(float deltaTime)
    {
        float speed = 200.0f;
        float jumpSpeed = 300.0f;

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
            //NYL_TRACE("Jump!");
            physics->jump(*Player, jumpSpeed, deltaTime);
            Player->getComponent<PhysicsComponent>()->canJump = false;
            //physics->applyGravity(*Player, deltaTime);
        }

        //physics->applyGravity(*Player, deltaTime);
    }

    void platformer::Quit()
    {
        NYL_TRACE("ANTARES quit");
    }
}

Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create platformer");
    return new platformer::platformer(1280, 720, "platformer");
    //return new platformer::platformer(1920, 1080, "platformer");
}