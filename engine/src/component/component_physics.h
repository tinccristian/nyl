#pragma once
#include <glm/glm.hpp>
#include "component.h"

/**
 * @brief The PhysicsComponent class represents a component that handles physics-related properties and behaviors for an entity.
 */
class NYL_API PhysicsComponent : public Component {
public:
    glm::vec2 velocity; /**< The velocity of the entity in 2D space. */
    float mass; /**< The mass of the entity. */
    bool canJump = false; /**< Whether or not the entity can jump. */
    float direction = 1.0f; /**< The direction the entity is facing. */
    /**
     * @brief Constructs a PhysicsComponent object with the specified parameters.
     * @param x The x-component of the initial velocity.
     * @param y The y-component of the initial velocity.
     * @param mass The mass of the entity.
     */
    PhysicsComponent(float x, float y, float mass) 
        : velocity(x, y), mass(mass) {}
};