#pragma once
#include "Core.h"
#include <glm/glm.hpp>

namespace Nyl{

/**
 * @class Collider
 * @brief A class representing a 2D collider.
 * 
 * This class provides functionality for detecting and handling 2D collisions.
 */
class NYL_API Collider {
public:
    glm::vec2 Position, Size, min, max; ///< Position, Size, min and max vectors of the collider.
    
    /**
     * @brief Construct a new Collider object.
     * 
     * @param posX X position of the collider.
     * @param posY Y position of the collider.
     * @param sizeX Width of the collider.
     * @param sizeY Height of the collider.
     */
    Collider(float posX, float posY, float sizeX, float sizeY);

    /**
     * @brief Check if this collider is colliding with another collider.
     * 
     * @param other The other collider to check collision with.
     * @return true If the colliders are colliding.
     * @return false Otherwise.
     */
    bool IsColliding(const Collider& other) const;

    /**
     * @brief Update the position of the collider.
     * 
     * @param position The new position of the collider.
     */
    void Update(const glm::vec2& position);

    /**
     * @brief Set the size of the collider.
     * 
     * @param size The new size of the collider.
     */
    void SetSize(const glm::vec2& size);

    /**
     * @brief Get the minimum point of the collider.
     * 
     * @return glm::vec2 The minimum point of the collider.
     */
    glm::vec2 GetMin() const;

    /**
     * @brief Get the maximum point of the collider.
     * 
     * @return glm::vec2 The maximum point of the collider.
     */
    glm::vec2 GetMax() const;

    /**
     * @brief Get the center point of the collider.
     * 
     * @return glm::vec2 The center point of the collider.
     */
    glm::vec2 GetCenter() const;

    /**
     * @brief Check if a point is contained within the collider.
     * 
     * @param point The point to check.
     * @return true If the point is within the collider.
     * @return false Otherwise.
     */
    bool ContainsPoint(const glm::vec2& point) const;

    /**
     * @brief Check if a line intersects the collider.
     * 
     * @param p1 The first point of the line.
     * @param p2 The second point of the line.
     * @return true If the line intersects the collider.
     * @return false Otherwise.
     */
    bool IntersectsLine(const glm::vec2& p1, const glm::vec2& p2) const;

    /**
     * @brief Check if a circle intersects the collider.
     * 
     * @param center The center of the circle.
     * @param radius The radius of the circle.
     * @return true If the circle intersects the collider.
     * @return false Otherwise.
     */
    bool IntersectsCircle(const glm::vec2& center, float radius) const;

    /**
     * @brief Check if an Axis-Aligned Bounding Box (AABB) intersects the collider.
     * 
     * @param min The minimum point of the AABB.
     * @param max The maximum point of the AABB.
     * @return true If the AABB intersects the collider.
     * @return false Otherwise.
     */
    bool IntersectsAABB(const glm::vec2& min, const glm::vec2& max) const;

    /**
     * @brief Check if an Oriented Bounding Box (OBB) intersects the collider.
     * 
     * @param position The position of the OBB.
     * @param size The size of the OBB.
     * @param rotation The rotation of the OBB.
     * @return true If the OBB intersects the collider.
     * @return false Otherwise.
     */
    bool IntersectsOBB(const glm::vec2& position, const glm::vec2& size, float rotation) const;
};
}