#pragma once

#include <nyl.h>
#include "collider.h"

namespace platformer
{
	class platformer : public nyl::Application
	{
	public:

		platformer(int width, int height, const std::string& title);
		~platformer();

		virtual void Init() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual void Quit() override;
		virtual void ProcessInput(float deltaTime) override;
		void LoadResources();
		void ConfigurePlayer();
		void CreateColliders();
		void CreateSystems();
		void HandleCollision(std::shared_ptr<Entity> player, std::shared_ptr<nyl::BoxCollider> collider, const nyl::CollisionInfo& collisionInfo);

		// struct for a point
        struct point {
            float x = 60.0f;
            float y = 280.0f;
        };
	};
}