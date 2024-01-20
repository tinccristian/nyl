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
        point startPoint;
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
    }

    void Antares::Update(float deltaTime)
    {
    // Draw background
    Renderer->DrawSprite(EntityManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);

    // Draw player
    Player->Draw(*Renderer);

    // Update player position
    Player->Position.x += .005;

    // Process input
    ProcessInput(deltaTime);

    //joystick recognition
    
    joystick->update();
    if (joystick->isPresent()) {
        NYL_TRACE("Joystick {0} is present", joystick->getName());
    }
    }

    void Antares::Quit()
    {
    NYL_TRACE("ANTARES quit");
    delete Renderer;
    delete Player;
    }

void Antares::ProcessInput(float deltaTime)
{



    // // Check if a joystick is connected
    // if (JoystickManager::isPresent(JoystickManager::JoystickId::JOYSTICK_1))
    // {
    //     NYL_TRACE("GAME Joystick 1 is present");

    //     // Get the axes values of the joystick
    //     int count;
    //     const float* axes = JoystickManager::getAxes(JoystickManager::JOYSTICK_1, &count);

    //     // Move the player based on joystick inputs
    //     float moveX = axes[0];
    //     float moveY = axes[1];

    //     // Update player position
    //     Player->Position.x += moveX;
    //     Player->Position.y += moveY;

    //     NYL_TRACE("Joystick input detected. MoveX: {0}, MoveY: {1}", moveX, moveY);
    // }
}
    
}
Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(1280, 720, "Antares");
}

