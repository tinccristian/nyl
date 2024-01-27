#include "system_collider.h"
using namespace Nyl;
void ColliderSystem::update() {
    for (Entity& entity : entities) {
        auto collider = entity.getComponent<ColliderComponent>();
        auto transform = entity.getComponent<TransformComponent>();
        if (collider && transform) {
            // Update the collider's position, size, min, and max based on the entity's position and size...
            collider->Update(transform->position);
        }
    }
}

bool ColliderSystem::isColliding(const ColliderComponent& a, const ColliderComponent& b) const {
    // Check if the two colliders are colliding...
    return a.Position.x < b.Position.x + b.Size.x &&
           a.Position.x + a.Size.x > b.Position.x &&
           a.Position.y < b.Position.y + b.Size.y &&
           a.Position.y + a.Size.y > b.Position.y;
}