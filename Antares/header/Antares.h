#pragma once

#include <nyl.h>

namespace Antares
{
	class Antares : public Nyl::Application
	{
	public:

		Antares(int width, int height, const std::string& title);
		~Antares();

		virtual void Init() override;
		virtual void Update(float deltaTime) override;
		virtual void Quit() override;
		virtual void ProcessInput(float deltaTime) override;
		void LoadResources();
		void ConfigurePlayer();
		void CreateColliders();
		void CreateSystems();
		void HandleCollision(std::shared_ptr<Entity> player, std::shared_ptr<Nyl::BoxCollider> collider);
	public:
		std::string workingPath = "D:/gitHub/nyl/nyl/";

	};
}