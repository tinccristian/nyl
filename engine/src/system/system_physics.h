#pragma once
#include "system.h"
#include "scene.h"
#include "transform.h"
#include "physics.h"
#include "collider.h"

namespace nyl
{
    class NYL_API PhysicsSystem : public System
    {
    public:
        static const float GRAVITY;

        /// Integrate velocity + gravity for every (Transform, Physics) entity.
        void update(Scene& scene, float deltaTime);

        void applyGravity(PhysicsComponent& physics, float deltaTime);
        void jump(PhysicsComponent& physics, float jumpSpeed);
        bool checkCollision(const BoxCollider& one, const BoxCollider& two);
    };
}
