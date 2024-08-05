#pragma once

#include <nyl.h>
#include "collider.h"

namespace platformer
{
	struct GameState
	{
		int level = 1;

	};
	class platformer : public nyl::Application
	{
	public:

		platformer(int width, int height, const std::string& title);
		~platformer();

		virtual void Init() override;
		virtual void Update(float deltaTime) override;
		virtual void Render(float deltaTime) override;
		virtual void Quit() override;
		virtual void ProcessInput(float deltaTime) override;
		void LoadResources();
		void ConfigurePlayer();
		void CreateColliders();
		void CreateSystems();
		void HandleCollision(std::shared_ptr<Entity> player, std::shared_ptr<nyl::BoxCollider> collider, const nyl::CollisionInfo& collisionInfo);

	public:
		GameState state;

	};
}