#include "Antares.h"

namespace Antares
{
    SpriteRenderer* Renderer;

    Antares::Antares(int width, int height, const std::string& title)
        : Nyl::Application(width, height, title)
    {
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
        // if (JoystickInput::IsJoystickConnected())
        // {
        //     // Get the axes values of the joystick
        //     std::vector<float> axes = JoystickInput::GetJoystickAxes();

        //     // Move the player based on joystick inputs
        //     float moveX = axes[JoystickAxisID::AXIS_1];
        //     float moveY = axes[JoystickAxisID::AXIS_2];

        //     // Update player position
        //     Player->Position.x += moveX;
        //     Player->Position.y += moveY;

        //     NYL_TRACE("Joystick input detected. MoveX: {0}, MoveY: {1}", moveX, moveY);
        // }
    }
    void Antares::JustAFoo()
    {
        NYL_TRACE("Just a foo");
    }
    
}
Nyl::Application* Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(1280, 720, "Antares");
}

    // void Antares::Update(float deltaTime)
    // {
    //     //auto bkg = EntityManager::GetTexture("background");
    //     Renderer->DrawSprite(EntityManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
    //     Player->Draw(*Renderer);
    //     Player->Position.x += .005;
    //     //NYL_TRACE("ANTARES update");
    //     ProcessInput(deltaTime);
    // }

    // void Antares::Quit()
    // {
    //     NYL_TRACE("ANTARES quit");
    //     delete Renderer;
    //     delete Player;
    // }
    // void Antares::ProcessInput(float deltaTime)
    // {
    //     // Check if a joystick is connected
	// 	if (JoystickInput::IsJoystickConnected())
	// 	{
	// 		// Get the axes values of the joystick
	// 		std::vector<float> axes = JoystickInput::GetJoystickAxes();

	// 		// Move the player based on joystick inputs
	// 		float moveX = axes[JoystickAxisID::AXIS_1];
	// 		float moveY = axes[JoystickAxisID::AXIS_2];

	// 		playerX += moveX;
	// 		playerY += moveY;
	// 	}
    // }