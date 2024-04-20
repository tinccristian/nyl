#pragma once

#include "system.h"
#include "collider.h"
#include "transform.h"
#include <optional>

namespace nyl {

    /**
     * @brief Represents the direction of collision.
     */
    enum class CollisionDirection {
        None,   ///< No collision
        Top,    ///< Collision from the top
        Bottom, ///< Collision from the bottom
        Left,   ///< Collision from the left
        Right   ///< Collision from the right
    };

    /**
     * @brief Holds collision information.
     */
    struct CollisionInfo {
        bool isColliding;           ///< Indicates if a collision is happening.
        CollisionDirection direction;   ///< The direction of the collision.

        /**
         * @brief Constructs a CollisionInfo object.
         *
         * @param colliding Indicates if a collision is happening.
         * @param dir The direction of the collision.
         */
        CollisionInfo(bool colliding, CollisionDirection dir) : isColliding(colliding), direction(dir) {}
    };

    /**
     * @brief Manages collision detection between entities with box colliders.
     */
    class NYL_API ColliderSystem : public System {
    public:
        /**
         * @brief Updates all entities in the system.
         *
         * This method should be called once per frame. It updates the position of all colliders based on the position of their entities.
         */
        void update() override;

        /**
         * @brief Checks if two colliders are colliding.
         *
         * @param a The first collider.
         * @param b The second collider.
         * @return An optional containing collision information if the colliders are colliding, or std::nullopt otherwise.
         */
        std::optional<CollisionInfo> isColliding(const BoxCollider& a, const BoxCollider& b) const;
    };

} // namespace nyl
