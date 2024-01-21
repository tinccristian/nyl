#include "Collider.h"

using namespace Nyl;
Collider::Collider(float posX, float posY, float sizeX, float sizeY)
    : Position(glm::vec2(posX, posY)), Size(glm::vec2(sizeX, sizeY))
{
    // Calculate min and max points based on position and size
    min = Position - Size / 2.0f;
    max = Position + Size / 2.0f;
}

bool Collider::IsColliding(const Collider& other) const
{
    // Check if this collider is colliding with the other collider
    // This is done by checking if there is any overlap in the x and y ranges of the two colliders
    return Position.x < other.Position.x + other.Size.x &&
           Position.x + Size.x > other.Position.x &&
           Position.y < other.Position.y + other.Size.y &&
           Position.y + Size.y > other.Position.y;
}

void Collider::Update(const glm::vec2& position)
{
    // Update the position of the collider
    Position = position;
    // Recalculate min and max points based on new position
    min = Position - Size / 2.0f;
    max = Position + Size / 2.0f;
}

void Collider::SetSize(const glm::vec2& size)
{
    // Set the size of the collider
    Size = size;
    // Recalculate min and max points based on new size
    min = Position - Size / 2.0f;
    max = Position + Size / 2.0f;
}

glm::vec2 Collider::GetMin() const
{
    // Return the minimum point of the collider
    return min;
}

glm::vec2 Collider::GetMax() const
{
    // Return the maximum point of the collider
    return max;
}

glm::vec2 Collider::GetCenter() const
{
    // Return the center point of the collider
    return Position;
}

bool Collider::ContainsPoint(const glm::vec2& point) const
{
    // Check if the point is inside the collider
    return point.x >= Position.x && point.x <= Position.x + Size.x &&
           point.y >= Position.y && point.y <= Position.y + Size.y;
}

bool Collider::IntersectsLine(const glm::vec2& p1, const glm::vec2& p2) const
{
    // Check if the line intersects with the collider
    // This can be done by checking if any of the line's endpoints are inside the collider
    return ContainsPoint(p1) || ContainsPoint(p2);
}

bool Collider::IntersectsCircle(const glm::vec2& center, float radius) const
{
    // Check if the circle intersects with the collider
    // This can be done by checking if the distance from the center of the circle to the closest point in the collider is less than the radius of the circle
    glm::vec2 closestPoint = glm::clamp(center, Position, Position + Size);
    return glm::distance(closestPoint, center) <= radius;
}

bool Collider::IntersectsAABB(const glm::vec2& min, const glm::vec2& max) const
{
    // Check if the axis-aligned bounding box intersects with the collider
    // This can be done by checking if there is any overlap in the x and y ranges of the two boxes
    return Position.x < max.x && Position.x + Size.x > min.x &&
           Position.y < max.y && Position.y + Size.y > min.y;
}

bool Collider::IntersectsOBB(const glm::vec2& position, const glm::vec2& size, float rotation) const
{
    // Check if the oriented bounding box intersects with the collider
    // This is a more complex problem that usually requires a separate algorithm
    // For simplicity, we will not implement this method here
    return false;
}
