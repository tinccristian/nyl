// ColliderSystem.cpp
#include "system_collider.h"
#include "log.h"
using namespace Nyl;

void ColliderSystem::update() {
    for (Entity& entity : entities) {
        auto boxCollider = entity.getComponent<BoxCollider>();
        auto transform = entity.getComponent<TransformComponent>();
        if (!boxCollider) {
            NYL_ERROR("Entity is missing BoxCollider component");
            continue;
        }
        if (!transform) {
            NYL_ERROR("Entity is missing TransformComponent");
            continue;
        }
        // Update the collider's min and max based on the entity's position and size...
        boxCollider->min = transform->min;
        boxCollider->max = transform->max;
    }
}

bool ColliderSystem::isColliding(const BoxCollider& a, const BoxCollider& b) const {
    // Check if the two colliders are colliding...
    // bool result = a.min.x < b.max.x &&
    //               a.max.x > b.min.x &&
    //               a.min.y < b.max.y &&
    //               a.max.y > b.min.y;
    // if(result)
    //     NYL_TRACE("Collision: {0}, {1}, {2}", a.flag, b.flag, result);
    return  a.min.x < b.max.x &&
            a.max.x > b.min.x &&
            a.min.y < b.max.y &&
            a.max.y > b.min.y;
}