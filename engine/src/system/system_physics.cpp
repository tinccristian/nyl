#include "system_physics.h"

namespace nyl
{
    const float PhysicsSystem::GRAVITY = 9.8f * 60.0f;

    void PhysicsSystem::update(Scene& scene, float deltaTime)
    {
        scene.forEach<TransformComponent, PhysicsComponent>(
            [&](EntityID, TransformComponent& transform, PhysicsComponent& physics)
            {
                transform.position += physics.velocity * deltaTime;
                transform.updateMinMax();
                physics.velocity.y += GRAVITY * deltaTime;
            });
    }

    void PhysicsSystem::applyGravity(PhysicsComponent& physics, float deltaTime)
    {
        physics.velocity.y += GRAVITY * deltaTime;
    }

    void PhysicsSystem::jump(PhysicsComponent& physics, float jumpSpeed)
    {
        physics.velocity.y = -jumpSpeed;
    }

    bool PhysicsSystem::checkCollision(const BoxCollider& one, const BoxCollider& two)
    {
        if (one.max.x < two.min.x || one.min.x > two.max.x) return false;
        if (one.max.y < two.min.y || one.min.y > two.max.y) return false;
        return true;
    }
}
