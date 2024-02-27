#pragma once
#include "system.h"
#include "component_collider.h"
#include "component_transform.h"

namespace Nyl{
    class NYL_API ColliderSystem : public System {
    public:
        /**
         * @brief Update all entities in the system.
         * 
         * This method should be called once per frame. It updates the position of all colliders based on the position of their entities.
         */
        void update() override;

        /**
         * @brief Check if two colliders are colliding.
         * 
         * @param a The first collider.
         * @param b The second collider.
         * @return true If the colliders are colliding.
         * @return false otherwise.
         */
        bool isColliding(const BoxCollider& a, const BoxCollider& b) const;
        };
}