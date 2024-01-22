#include "Physics.h"
#include "Log.h"
using namespace Nyl;

const float Physics::GRAVITY = 9.8f*60.0f;

void Physics::UpdatePosition(GameObject* object, float deltaTime, float screenWidth) {
    // Calculate the new position based on the object's velocity and deltaTime
    glm::vec2 velocity = object->Velocity;
    glm::vec2 position = object->Position;
    glm::vec2 size = object->Size;
    position += velocity * deltaTime;

    // Check if the object has reached the boundaries of the screen
    //float objectWidth = 128.0f; // Assuming the object has a width property
    if (position.x < size.x / 2) {
        position.x = size.x / 2;
        velocity.x = 0; // Stop the object's horizontal movement
        //NYL_CORE_INFO("COLLISION LEFT");
    } else if (position.x > screenWidth - size.x / 2) {
        position.x = screenWidth - size.x / 2;
        velocity.x = 0; // Stop the object's horizontal movement
        //NYL_CORE_INFO("COLLISION RIGHT Screen Width {0} -  size.x {1}, Position.x {2}", screenWidth, size.x, position.x);
    }

    // Update the object's position and velocity
    object->Position = position;
    object->Velocity = velocity;
}

void Physics::ApplyGravity(GameObject* object, float deltaTime) {
    // Calculate the new velocity based on gravity and deltaTime
    glm::vec2 velocity = object->Velocity;
    velocity.y += GRAVITY * deltaTime;

    // Update the object's velocity
    object->Velocity = velocity;
}

void Physics::Jump(GameObject* object, float jumpSpeed,float deltaTime) {
    // Apply the jump speed to the object's velocity in the y-axis
    object->Velocity.y = -jumpSpeed+GRAVITY*deltaTime;
}

void Physics::ApplyForce(GameObject* object) {
    // Implementation here...
    // Apply the force to the object
    // ...
}

bool Physics::CheckCollision(const Collider& one, const Collider& two) {
    // Check if there is a collision between the two AABB colliders
    if (one.max.x < two.min.x || one.min.x > two.max.x) {
        return false; // No collision in the x-axis
    }
    if (one.max.y < two.min.y || one.min.y > two.max.y) {
        return false; // No collision in the y-axis
    }

    // Return true if there is a collision in both x-axis and y-axis
    return true;
}

void Physics::ApplyFriction(GameObject* object, float friction) {
    // Implementation here...
}

void Physics::ApplyImpulse(GameObject* object, const glm::vec2& impulse) {
    // Implementation here...
}

void Physics::ApplyTorque(GameObject* object, float torque) {
    // Implementation here...
}

void Physics::ApplyAngularImpulse(GameObject* object, float impulse) {
    // Implementation here...
}

void Physics::ApplyDrag(GameObject* object, float dragCoefficient) {
    // Implementation here...
}

void Physics::ApplyBuoyancy(GameObject* object, float fluidDensity, float fluidLevel) {
    // Implementation here...
}
