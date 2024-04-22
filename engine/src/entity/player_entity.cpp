#include "player_entity.h"

namespace nyl
{
	PlayerEntity::PlayerEntity(
								const TransformComponent &transform,
								const PhysicsComponent   &physics,
								const BoxCollider        &collider,
								const TextureComponent   &texture,
								const Camera			 &camera):
								transform(transform),
								physics(physics),
								collider(collider),
								texture(texture),
								camera(camera)
	{
		this->addComponent<TransformComponent>(transform);
		this->addComponent<PhysicsComponent>(physics);
		this->addComponent<BoxCollider>(collider);
		this->addComponent<TextureComponent>(texture);
		this->addComponent<Camera>(camera);
	}
}
