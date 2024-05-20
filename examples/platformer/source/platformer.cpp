#include "platformer.h"
#include "input.h"
#include "system_renderer.h"
#include "camera.h"
#include "system_camera.h"

using namespace nyl;
namespace platformer
{

    //std::shared_ptr<Entity> Player; 
    std::shared_ptr<PlayerEntity> Player; 
    std::vector<std::shared_ptr<BoxCollider>> colliders;
    std::unique_ptr<CameraSystem> cameraManager;
    std::unique_ptr<RenderSystem> Renderer;
    std::unique_ptr<RenderSystem> debugRenderer;
    std::unique_ptr<PhysicsSystem> physics;
    std::unique_ptr<ColliderSystem> collisions;
    std::unique_ptr<Joystick> joystick;
	TextureComponent* background;

    float cloudTimer = 0.0f;
    const float cloudInterval = 5.0f;
    bool isInAir = false;

    platformer::platformer(int width, int height, const std::string& title)
        : nyl::Application(width, height, title)
    {
    }

    platformer::~platformer()
    {
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

        //NYL_INFO("Player->transform->position.x,y: {0}, {1} ", Player->transform.position.x, Player->transform.position.y);
        //NYL_INFO("Player->getComponent<TransformComponent>()->position.x,y : {0}, {1} ", Player->getComponent<TransformComponent>()->position.x, Player->getComponent<TransformComponent>()->position.y);
		physics->updatePhysics(deltaTime);

		// Update camera
		cameraManager->update(*Player);
		// Process input
		ProcessInput(deltaTime);

		// Update collider
		collisions->update();

		// Check collisions
		bool collisionDetected = false;
		for (auto& worldCollider : colliders)
		{
			auto collisionInfo = collisions->isColliding(*Player->getComponent<BoxCollider>(), *worldCollider);
			if (collisionInfo.has_value())
			{
				if (worldCollider->flag == "platform")
				{
					HandleCollision(Player, worldCollider, collisionInfo.value());
                    collisionDetected = true;
					break;
				}
				else if (worldCollider->flag == "level2")
				{
					NYL_INFO("Level 2 reached");
                    state.level++;
                    Player->getComponent<TransformComponent>()->position.x = 0.0f;
                    Player->getComponent<TransformComponent>()->position.y = 0.0f;
                    Player->getComponent<PhysicsComponent>()->velocity = glm::vec2(0, 0);
				}
			}
		}
		if (!collisionDetected)
		{
			Player->getComponent<PhysicsComponent>()->canJump = false;
		}
	}

void platformer::Render()
{


    switch (state.level)
	{
	case 1:
		background = ResourceManager::GetTexture("lv1");
        break;

    case 2:
		background = ResourceManager::GetTexture("lv2");
        break;
    }

    Renderer->DrawSprite(*background, glm::vec2(0.0f,0.0f),glm::vec2(this->m_window->width, this->m_window->height));

    // Draw clouds
    TextureComponent* cloud = ResourceManager::GetTexture("cloud");
    Renderer->DrawSprite(*cloud, glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f));
    Renderer->DrawSprite(*cloud, glm::vec2(250.0f, 15.0f), glm::vec2(100.0f, 100.0f));
    Renderer->DrawSprite(*cloud, glm::vec2(454.0f,32.0f), glm::vec2(100.0f, 100.0f));
    Renderer->DrawSprite(*cloud, glm::vec2(790.0f,80.0f), glm::vec2(100.0f, 100.0f));
    Renderer->DrawSprite(*cloud, glm::vec2(1100.0f, 150.0f), glm::vec2(100.0f, 100.0f));

    //Draw player
    Renderer->DrawEntity(*Player);
}
#pragma region init_helper_foo
    void platformer::LoadResources()
    {
        std::string resourcePath = getFullPath("../../resources/");

        std::vector<std::string> texturePaths = {
            resourcePath + "backgrounds/lv1.png",
            resourcePath + "backgrounds/lv2.png",
            resourcePath + "characters/chikboy_trim.png",// should be moved engine side as default texture (game.h)
            resourcePath + "backgrounds/cloud.png"
        };

        for (const std::string& path : texturePaths) {
            std::string textureName = path.substr(path.find_last_of('/') + 1);
            textureName = textureName.substr(0, textureName.find_last_of('.'));
            if (!ResourceManager::LoadTexture(path.c_str(), true, textureName)) {
                NYL_ERROR("Failed to load texture: {}", textureName);
            }
        }
    }
    void platformer::ConfigurePlayer()
    {

        Player = std::make_shared<PlayerEntity>();

        NYL_TRACE("TransformComponent: {0}",Player->hasComponent<TransformComponent>());
        NYL_TRACE("PhysicsComponent: {0}",Player->hasComponent<PhysicsComponent>());
        NYL_TRACE("BoxCollider: {0}",Player->hasComponent<BoxCollider>());
        NYL_TRACE("TextureComponent: {0}",Player->hasComponent<TextureComponent>());
        NYL_TRACE("Camera: {0}",Player->hasComponent<Camera>());

    }
    void platformer::CreateColliders()
    {
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(0,this->m_window->height), glm::vec2(this->m_window->width, this->m_window->height -5.0f), "platform"));//ground
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(0,384),   glm::vec2(364,335), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(544,714), glm::vec2(798,458), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(309,202), glm::vec2(358,153), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(163,80),  glm::vec2(212,31) , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(429,524), glm::vec2(478,475), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(291,614), glm::vec2(340,565), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(542,282), glm::vec2(796,246), "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(615,245), glm::vec2(664,5)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(872,524), glm::vec2(922,475)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(1077,650), glm::vec2(1126,601)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(907,386), glm::vec2(1274,335)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(945,192), glm::vec2(994,143)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(1169,66), glm::vec2(1218,17)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(712,84), glm::vec2(754,5)  , "platform"));
        colliders.push_back(std::make_shared<BoxCollider>(glm::vec2(1255,715), glm::vec2(1276,0), "level2"));
    }
    void platformer::CreateSystems()
    {
            ShaderComponent* spriteShader = ResourceManager::GetShader("sprite");
            Renderer = std::make_unique<RenderSystem>(*spriteShader,this->m_window->width,this->m_window->height);

            ShaderComponent* debugShader = ResourceManager::GetShader("debug");
            debugRenderer = std::make_unique<RenderSystem>(*debugShader,this->m_window->width,this->m_window->height);

            cameraManager = std::make_unique<CameraSystem>(std::make_shared<Camera>(Player->camera));
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
	void platformer::HandleCollision(std::shared_ptr<Entity> player, std::shared_ptr<BoxCollider> collider, const CollisionInfo& collisionInfo)
	{
		auto p_Transform = player->getComponent<TransformComponent>();
		auto p_Physics = player->getComponent<PhysicsComponent>();

		// Update player's position and velocity based on collision direction
		switch (collisionInfo.direction)
		{
		case CollisionDirection::Left:
			p_Transform->position.x = collider->min.x - p_Transform->size.x;
			p_Physics->velocity.x = 0;
			break;
		case CollisionDirection::Right:
			p_Transform->position.x = collider->max.x;
			p_Physics->velocity.x = 0;
			break;
		case CollisionDirection::Bottom:
            p_Transform->position.y = collider->min.y +p_Transform->size.y-11.0f;
			p_Physics->velocity.y = 0;
			break;
		case CollisionDirection::Top:
			p_Transform->position.y = collider->min.y - p_Transform->size.y;
			p_Physics->velocity.y = 0;
			p_Physics->canJump = true;
            isInAir = false;
			break;
		}
	}

    void platformer::ProcessInput(float deltaTime)
    {
        float speed = 200.0f;
        float jumpSpeed = 450.0f;

        joystick->update();

        if (!joystick->isPresent()) 
        {
            NYL_TRACE("Joystick not present");
            return;
        }

        auto moveX = joystick->axesState(0);
        auto jumpButton = joystick->buttonState(GLFW_JOYSTICK_BTN_DOWN) || joystick->buttonState(GLFW_JOYSTICK_BTN_LEFT);

        if (std::abs(moveX) > 0.1) 
        {
			Player->getComponent<PhysicsComponent>()->velocity.x = (moveX > 0) ? speed : (moveX < 0) ? -speed : 0;
            Player->getComponent<PhysicsComponent>()->direction = moveX >= 0 ? 1.0f : -1.0f;
            Player->getComponent<TransformComponent>()->direction = moveX >= 0 ? 1.0f : -1.0f;
        }
        else 
        {
            Player->getComponent<PhysicsComponent>()->velocity.x = 0.0f;
        }

        float groundLevel = this->m_window->height - Player->getComponent<TransformComponent>()->size.y;
        float tolerance = 10.0f;

        if (jumpButton && Player->getComponent<PhysicsComponent>()->canJump) {
            //NYL_TRACE("Jump!");
            physics->jump(*Player, jumpSpeed, deltaTime);
            Player->getComponent<PhysicsComponent>()->canJump = false;
            isInAir = true;
            Player->getComponent<PhysicsComponent>()->velocity.x /= 2.0f;
        }
        else if (!jumpButton && isInAir)
        {
            physics->applyGravity(*Player, deltaTime);
        }
    }

    void platformer::Quit()
    {
        NYL_TRACE("app quit");
    }
}

nyl::Application* nyl::CreateApplication()
{
    NYL_TRACE("Create platformer");
    return new platformer::platformer(1280, 720, "platformer");
    //return new platformer::platformer(1920, 1080, "platformer");
}