#pragma once

#include "entity.h"
#include "resource_manager.h"

#include "transform.h"
#include "physics.h"
#include "collider.h"
#include "texture.h"
#include "camera.h"

#include "glm/glm.hpp"

namespace nyl 
{
	class NYL_API PlayerEntity: public Entity
	{
	public:

		PlayerEntity(
					 const TransformComponent &transform = TransformComponent (0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 32.0f, 64.0f),
					 const PhysicsComponent   &physics   = PhysicsComponent   (1.0f, 2.0f, 50.0f),
					 const BoxCollider        &collider  = BoxCollider        (glm::vec2(0.0f,0.0f), glm::vec2(36.0f,64.0f), "player"),
					 const TextureComponent   &texture   = TextureComponent   (*ResourceManager::GetTexture("chikboy_trim")),
					 const Camera             &camera    = Camera             (0.0f, 0.0f, 800.0f, 600.0f, 0.5f)
					);
		~PlayerEntity()= default;

	public:

		//should be unique_ptrs?
		TransformComponent transform;
		PhysicsComponent physics;
		BoxCollider collider;
		TextureComponent texture;
		Camera camera;
	};
}