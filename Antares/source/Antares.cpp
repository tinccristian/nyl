#include "Antares.h"
#include "Input.h"
namespace Antares
{
    SpriteRenderer* Renderer;
    Joystick* joystick;

    Antares::Antares(int width, int height, const std::string& title)
        : Nyl::Application(width, height, title)
    {
        //joystick = new Nyl::Joystick(0); // Initialize joystick with id 0
        NYL_TRACE("ANTARES constructor");
    }

    Antares::~Antares()
    {
        delete joystick;
        delete Player;
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
        float posX = width / 2.0f - sizeX;
        float posY = height / 2.0f - sizeY;
        point startPoint={60.0f, 280.0f};
        NYL_WARN("Rendering chikboy at x = {0}, y = {1}", startPoint.x, startPoint.y);

        Player = new GameObject(
            startPoint.x,
            startPoint.y,
            sizeX, sizeY,
            EntityManager::GetTexture("chikboy"));

        // Set render specific controls
        Shader spriteShader = EntityManager::GetShader("sprite");
        Renderer = new SpriteRenderer(spriteShader);
        joystick = new Joystick(0);
        if (joystick->isPresent()) {NYL_TRACE("Joystick {0} is present", joystick->getName());}
    }

    void Antares::Update(float deltaTime)
    {
        // Draw background
        Renderer->DrawSprite(EntityManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);

        // Draw player
        Player->Draw(*Renderer);

        // Process input
        ProcessInput(deltaTime);

    }

    void Antares::Quit()
    {
    NYL_TRACE("ANTARES quit");
    delete Renderer;
    delete Player;
    }

void Antares::ProcessInput(float deltaTime)
{
    // Joystick recognition
    float speed = 150.0f;
    float jumpSpeed = 500.0f; // Initial speed at which the player jumps
    float gravity = 9.8f * 60; // Gravity, multiplied by 60 to make it work well with deltaTime

    joystick->update();
    if (joystick->isPresent()) {
        auto moveX = joystick->axesState(0);
        auto jumpButton = joystick->buttonState(GLFW_JOYSTICK_BTN_DOWN) || joystick->buttonState(GLFW_JOYSTICK_BTN_LEFT);

        if (std::abs(moveX) > 0.1)
        {
            float newX = Player->Position.x + moveX * deltaTime * speed;
            if (newX >= -15 && newX <= width - Player->Size.x + 20)
            {
                Player->Position.x = newX;
                //NYL_TRACE("Moving playerX by {0}", std::to_string(moveX));
            }
        }

        if (jumpButton && Player->Position.y >= height - Player->Size.y)
        {
            Player->Velocity.y = -jumpSpeed; // Make the player jump
        }

        Player->Velocity.y += gravity * deltaTime; // Apply gravity

        float newY = Player->Position.y + Player->Velocity.y * deltaTime;
        if (newY >= 0 && newY <= height - Player->Size.y)
        {
            Player->Position.y = newY;
            //NYL_TRACE("Moving playerY by {0}", std::to_string(moveY));
        }
        else
        {
            if (newY < 0)
            {
                Player->Position.y = 0;
            }
            else
            {
                Player->Position.y = height - Player->Size.y;
                Player->Velocity.y = 0; // Stop the player's downward velocity
            }
        }
    }
    else
    {
        NYL_TRACE("Joystick not present");
    }
}

}
Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(1280, 720, "Antares");
}

