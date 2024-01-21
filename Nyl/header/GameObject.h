#pragma once
#include "Core.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Renderer.h"
#include "Physics.h" // Include the Physics class
#include "Collider.h" // Include the Collider class

using namespace Nyl;

class Physics; // Forward declaration of the Physics class
/**
 * @class GameObject
 * @brief Represents a game object in the game world.
 * 
 * The GameObject class encapsulates the properties and behaviors of a game object,
 * such as its position, size, velocity, color, rotation, and render state.
 * It also contains optional physics and collider components for handling physics simulation and collision detection.
 */
class NYL_API GameObject
{
public:
    glm::vec2   Position, Size, Velocity; /**< The position, size, and velocity of the game object. */
    glm::vec3   Color; /**< The color of the game object. */
    float       Rotation; /**< The rotation angle of the game object. */
    bool        IsSolid; /**< Indicates whether the game object is solid or not. */
    bool        Destroyed; /**< Indicates whether the game object is destroyed or not. */
    bool       canJump; /**< Indicates whether the game object can jump or not. */
    float      jumpHeight; /**< The jump height of the game object. */
    float     jumpSpeed; /**< The jump speed of the game object. */
    float     gravity; /**< The gravity of the game object. */
    Texture   Sprite; /**< The sprite texture of the game object. */
    Physics* physicsComponent; /**< Pointer to the Physics component. */
    Collider* colliderComponent; /**< Pointer to the Collider component. */

    /**
     * @brief Constructs a GameObject object.
     * @param x The x-coordinate of the game object's position.
     * @param y The y-coordinate of the game object's position.
     * @param sizeX The width of the game object.
     * @param sizeY The height of the game object.
     * @param sprite The sprite texture of the game object.
     * @param color The color of the game object.
     * @param velocity The velocity of the game object.
     * @param physics Optional pointer to the Physics component.
     * @param collider Optional pointer to the Collider component.
     */
    GameObject(
        float x = 0.0f, float y = 0.0f,
        float sizeX = 1.0f, float sizeY = 1.0f,
        Texture sprite = Texture(),
        glm::vec3 color = glm::vec3(1.0f),
        glm::vec2 velocity = glm::vec2(0.0f, 0.0f),
        Physics* physics = nullptr,
        Collider* collider = nullptr);

    /**
     * @brief Draws the game object's sprite using the specified renderer.
     * @param renderer The SpriteRenderer used to draw the sprite.
     */
    virtual void Draw(SpriteRenderer& renderer);
};
