#pragma once
#include "system.h"
#include "component_transform.h"
#include "component_physics.h"
#include "component_collider.h"

class NYL_API PhysicsSystem : public System {
public:
    static const float GRAVITY;

    void updatePhysics(float deltaTime, float width);
    void applyGravity(Entity& entity, float deltaTime);
    void jump(Entity& entity, float jumpSpeed, float deltaTime);
    bool checkCollision(const Nyl::BoxCollider& one, const Nyl::BoxCollider& two);

    void applyFriction(Entity& entity, float friction);
    void applyImpulse(Entity& entity, const glm::vec2& impulse);
    void applyTorque(Entity& entity, float torque);
    void applyAngularImpulse(Entity& entity, float impulse);
    void applyDrag(Entity& entity, float dragCoefficient);
    void applyBuoyancy(Entity& entity, float fluidDensity, float fluidLevel);
};