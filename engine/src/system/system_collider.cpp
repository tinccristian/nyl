// ColliderSystem.cpp
#include "system_collider.h"
#include "log.h"
using namespace nyl;

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

std::optional<CollisionInfo> ColliderSystem::isColliding(const BoxCollider& a, const BoxCollider& b) const {
	// Check if the two colliders are colliding...
	bool result = a.min.x < b.max.x &&
		a.max.x > b.min.x &&
		a.min.y < b.max.y &&
		a.max.y > b.min.y;

	if (result) {
		// Calculate the overlap in each direction
		float dx1 = b.max.x - a.min.x; // Distance from left side of 'a' to right side of 'b'
		float dx2 = a.max.x - b.min.x; // Distance from left side of 'b' to right side of 'a'
		float dy1 = b.max.y - a.min.y; // Distance from bottom side of 'a' to top side of 'b'
		float dy2 = a.max.y - b.min.y; // Distance from bottom side of 'b' to top side of 'a'

		// Determine the direction of collision based on the smallest overlap
		CollisionDirection direction;
		if (std::min(dx1, dx2) < std::min(dy1, dy2)) {
			if (dx1 < dx2) {
				// Collision from the left side of 'a' to right side of 'b'
				direction = CollisionDirection::Right;
				NYL_CORE_TRACE("Collision from the right");
			}
			else {
				// Collision from the right side of 'a' to left side of 'b'
				direction = CollisionDirection::Left;
				NYL_CORE_TRACE("Collision from the left");
			}
		}
		else {
			if (dy1 < dy2) {
				// Collision from the bottom side of 'a' to top side of 'b'
				direction = CollisionDirection::Bottom;
				NYL_CORE_TRACE("Collision from the bottom");
			}
			else {
				// Collision from the top side of 'a' to bottom side of 'b'
				direction = CollisionDirection::Top;
				NYL_CORE_TRACE("Collision from the top");
			}
		}

		//NYL_TRACE("Collision: {0}, {1}", a.flag, b.flag);
		// Return the collision information
		return CollisionInfo(true, direction);
	}
	return std::nullopt; // No collision
}