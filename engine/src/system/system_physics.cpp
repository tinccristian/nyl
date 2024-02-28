#include "system_physics.h"
#include "component_physics.h"
#include "component_transform.h"

const float PhysicsSystem::GRAVITY = 9.8f*60.0f;

void PhysicsSystem::updatePhysics(float deltaTime, float width) {
    for (Entity& entity : entities) {
        auto transform = entity.getComponent<TransformComponent>();
        auto physics = entity.getComponent<PhysicsComponent>();
        if (transform && physics) {
            transform->position += physics->velocity * deltaTime;
            transform->updateMinMax();
            if (transform->position.x < 0) {
                transform->position.x = 0;
                physics->velocity.x = 0;
            } else if (transform->position.x > width - transform->size.x) {
                transform->position.x = width - transform->size.x;
                physics->velocity.x = 0;
            }

            applyGravity(entity, deltaTime);
        }
    }
}

void PhysicsSystem::applyGravity(Entity& entity, float deltaTime) {
    auto physics = entity.getComponent<PhysicsComponent>();
    if (physics) {
        physics->velocity.y += GRAVITY * deltaTime;
    }
}

void PhysicsSystem::jump(Entity& entity, float jumpSpeed, float deltaTime) {
    auto physics = entity.getComponent<PhysicsComponent>();
    if (physics) {
        physics->velocity.y = -jumpSpeed + GRAVITY * deltaTime;
    }
}

bool PhysicsSystem::checkCollision(const Nyl::BoxCollider& one, const Nyl::BoxCollider& two) {
    if (one.max.x < two.min.x || one.min.x > two.max.x) {
        return false;
    }
    if (one.max.y < two.min.y || one.min.y > two.max.y) {
        return false;
    }
    return true;
}

void PhysicsSystem::applyFriction(Entity& entity, float friction) {
    // Implementation here...
}

void PhysicsSystem::applyImpulse(Entity& entity, const glm::vec2& impulse) {
    // Implementation here...
}

void PhysicsSystem::applyTorque(Entity& entity, float torque) {
    // Implementation here...
}

void PhysicsSystem::applyAngularImpulse(Entity& entity, float impulse) {
    // Implementation here...
}

void PhysicsSystem::applyDrag(Entity& entity, float dragCoefficient) {
    // Implementation here...
}

void PhysicsSystem::applyBuoyancy(Entity& entity, float fluidDensity, float fluidLevel) {
    // Implementation here...
}