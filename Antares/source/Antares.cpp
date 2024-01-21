#include "Antares.h"
#include "Input.h"
namespace Antares
{
    SpriteRenderer* Renderer;
    SpriteRenderer* debugRenderer;
    Joystick* joystick;
    Physics* physics;
    GameObject* Player; // Declare the "Player" variable
    Collider* collider;
    Collider* collider_platform;
    Collider* collider_platform_1;
    Collider* collider_platform_2;


    Antares::Antares(int width, int height, const std::string& title)
        : Nyl::Application(width, height, title)
    {
        //joystick = new Nyl::Joystick(0); // Initialize joystick with id 0
        NYL_TRACE("ANTARES constructor");

    }

    Antares::~Antares()
    {
        delete collider;
        delete physics;
        delete joystick;
        delete Player;
        delete debugRenderer;
        delete Renderer;
        NYL_TRACE("ANTARES destructor");
    }

    void Antares::Init()
    {
        NYL_TRACE("ANTARES init");

        // Load textures
        EntityManager::LoadTexture("D:/gitHub/nyl/Nyl/resources/chikboy/chikboy.png", true, "chikboy");
        EntityManager::LoadTexture("D:/gitHub/nyl/Nyl/resources/backgrounds/02.png", true, "background");

        // Configure game objects
        float sizeY = 64.0f;
        float sizeX = 64.0f;
        // float posX = width / 2.0f - sizeX;
        // float posY = height / 2.0f - sizeY;
        point startPoint={60.0f, 280.0f};
        NYL_WARN("Rendering chikboy at x = {0}, y = {1}", startPoint.x, startPoint.y);
        // Create Physics and Collider components
        physics = new Physics();
        collider = new Collider(startPoint.x, startPoint.y, sizeX, sizeY);

        // Manually fill collider_platform's fields
        float platformWidth = 400.0f;
        float platformHeight = 50.0f;
        float platformPosX = startPoint.x;
        float platformPosY = startPoint.y + sizeY + 100.0f; // 50 pixels below the player
        
        collider_platform = new Collider(platformPosX, platformPosY, platformWidth, platformHeight);
        collider_platform_1 = new Collider(platformPosX+100, platformPosY-100, platformWidth, platformHeight);
        collider_platform_2 = new Collider(platformPosX+500, platformPosY+200, platformWidth, platformHeight);
        
        Player = new GameObject(
            startPoint.x,
            startPoint.y,
            sizeX, sizeY,
            EntityManager::GetTexture("chikboy"),
            glm::vec3(1.0f), // default color
            glm::vec2(0.0f), // default velocity
            physics,
            collider);
        // Set render specific controls
        Shader spriteShader = EntityManager::GetShader("sprite");
        Renderer = new SpriteRenderer(spriteShader);
            // Set render specific controls
        Shader debugShader = EntityManager::GetShader("debug");
        debugRenderer = new SpriteRenderer(debugShader);
        joystick = new Joystick(0);
        if (joystick->isPresent()) {NYL_TRACE("Joystick {0} is present", joystick->getName());}
    }

    void Antares::Update(float deltaTime)
    {
        // Draw background
        Renderer->DrawSprite(EntityManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);

        // Process input

        // Draw player

        // Debug draw
        collider->Update(Player->Position);
        debugRenderer->DrawRectangleOutline(collider->Position, collider->Size, 0.0f, Colors::Green.value);
        debugRenderer->DrawRectangleOutline(collider_platform_1->Position, collider_platform_1->Size, 0.0f, Colors::Blue.value);
        debugRenderer->DrawRectangleOutline(collider_platform_2->Position, collider_platform_2->Size, 0.0f, Colors::Blue.value);
        Player->Draw(*Renderer);

        debugRenderer->DrawRectangleOutline(collider_platform->Position, collider_platform->Size, 0.0f, Colors::Red.value);

        if(collider_platform->IsColliding(*collider))
        {
            Player->Position.y = collider_platform->Position.y - Player->Size.y;
            Player->Velocity.y = 0; // Stop the player's downward velocity
            Player->canJump = true; // Reset jump state
            //NYL_TRACE("Player is colliding with platform");
        }
        else if(collider_platform_1->IsColliding(*collider))
        {
            Player->Position.y = collider_platform_1->Position.y - Player->Size.y;
            Player->Velocity.y = 0; // Stop the player's downward velocity
            Player->canJump = true; // Reset jump state
            //NYL_TRACE("Player is colliding with platform");
        }
        else if(collider_platform_2->IsColliding(*collider))
        {
            Player->Position.y = collider_platform_2->Position.y - Player->Size.y;
            Player->Velocity.y = 0; // Stop the player's downward velocity
            Player->canJump = true; // Reset jump state
            //NYL_TRACE("Player is colliding with platform");
        }
        else 
        {
            //NYL_TRACE("Player is not colliding with platform");
        }
        ProcessInput(deltaTime);

    }



    void Antares::ProcessInput(float deltaTime)
    {
        // Define movement and jump speeds
        float speed = 150.0f;
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
            glm::vec2 newPosition = Player->Position + glm::vec2(moveX * deltaTime * speed, 0.0f);
            if (newPosition.x >= 0 && newPosition.x <= width - Player->Size.x)
            {
                Player->Position = newPosition;
                physics->UpdatePosition(Player, deltaTime, width);
            }
            //else{NYL_WARN("Player is out of bounds");}
        }

        // Apply gravity
        physics->ApplyGravity(Player, deltaTime);

        // Process jumping
        float groundLevel = height - Player->Size.y;
        float tolerance = 10.0f;
        if (jumpButton && ((Player->Position.y >= groundLevel - tolerance) || Player->canJump))
        {
            physics->Jump(Player, jumpSpeed, deltaTime);
            physics->UpdatePosition(Player, deltaTime, height);
            Player->canJump = false; // Reset jump state
        }

        // Process vertical movement
        glm::vec2 newPosition = Player->Position + Player->Velocity * deltaTime;
        if (newPosition.y >= 0 && newPosition.y <= height - Player->Size.y)
        {
            Player->Position = newPosition;
            physics->UpdatePosition(Player, deltaTime, height);
        }
        else
        {
            // Handle collision with top and bottom of screen
            Player->Position.y = (newPosition.y < 0) ? 0 : height - Player->Size.y;
            Player->Velocity.y = 0; // Stop the player's downward velocity
            physics->UpdatePosition(Player, deltaTime, height);
        }
    }
    void Antares::Quit()
    {
        NYL_TRACE("ANTARES quit");
        delete Renderer;
        delete Player;
    }
}
Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(1280, 720, "Antares");
}

