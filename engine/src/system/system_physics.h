#pragma once
#include "system.h"
#include "transform.h"
#include "physics.h"
#include "collider.h"

class NYL_API PhysicsSystem : public System {
public:
    static const float GRAVITY;

    void updatePhysics(float deltaTime);
    void applyGravity(Entity& entity, float deltaTime);
    void jump(Entity& entity, float jumpSpeed, float deltaTime);
    bool checkCollision(const nyl::BoxCollider& one, const nyl::BoxCollider& two);
};