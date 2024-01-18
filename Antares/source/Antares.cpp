#include "../../Antares/Antares.h"

namespace Antares
{
    using namespace Nyl;
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
        EntityManager::LoadTexture("D:/gitHub/nyl/Nyl/resources/chikboy/chikboy.png", true, "chikboy");
        EntityManager::LoadTexture("D:/gitHub/nyl/Nyl/resources/backgrounds/02.png", true, "background");

        // configure game objects
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
        //glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y); //constructor above is easier to use in dllexport without needing glm
        //Player = new GameObject(playerPos, PLAYER_SIZE, EntityManager::GetTexture("chikboy")); 
        // set render specific controls
        Shader spriteShader = EntityManager::GetShader("sprite");
        Renderer = new SpriteRenderer(spriteShader);
    }

    void Antares::Update()
    {
        //auto bkg = EntityManager::GetTexture("background");
        Renderer->DrawSprite(EntityManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        Player->Draw(*Renderer);
        Player->Position.x += .005;
        //NYL_TRACE("ANTARES update");
    }

    void Antares::Quit()
    {
        NYL_TRACE("ANTARES quit");
        delete Renderer;
        delete Player;
    }
}
Application * Nyl::CreateApplication()
{
    NYL_TRACE("Create Antares");
    return new Antares::Antares(1280, 720, "Antares");
}