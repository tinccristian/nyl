#pragma once
#include "Core.h"
#include "GameObject.h"
#include "Collider.h"

using namespace Nyl;
class GameObject; // Forward declaration

class NYL_API Physics {
public:
    static const float GRAVITY;

    static void UpdatePosition(GameObject* object, float deltaTime, float width);
    static void ApplyGravity(GameObject* object, float deltaTime);
    static void Jump(GameObject* object, float jumpSpeed, float deltaTime);
    static void ApplyForce(GameObject* object);
    static bool CheckCollision(const Collider& one, const Collider& two);

    static void ApplyFriction(GameObject* object, float friction);
    static void ApplyImpulse(GameObject* object, const glm::vec2& impulse);
    static void ApplyTorque(GameObject* object, float torque);
    static void ApplyAngularImpulse(GameObject* object, float impulse);
    static void ApplyDrag(GameObject* object, float dragCoefficient);
    static void ApplyBuoyancy(GameObject* object, float fluidDensity, float fluidLevel);
};
