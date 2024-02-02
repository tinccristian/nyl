// BoxColliderComponent.h
#pragma once
#include "component.h"
#include <glm/glm.hpp>
#include <string>

namespace Nyl
{
    /**
     * @brief Represents a box collider component.
     * 
     * The BoxCollider class is a derived class of the Component class and represents a box-shaped collider in the nyl game engine.
     * It defines a rectangular region in 2D space defined by a minimum and maximum vector.
     * The collider can be associated with a flag for identification purposes.
     */
    class NYL_API BoxCollider : public Component {
    public:
        glm::vec2 min, max; ///< Min and max vectors of the collider.
        std::string flag;
        bool isColliding=false;

        /**
         * @brief Construct a new BoxCollider object from two points.
         * 
         * @param min The bottom-left corner of the box.
         * @param max The top-right corner of the box.
         * @param flag The flag of the collider.
         */
        BoxCollider(const glm::vec2& bottomLeft, const glm::vec2& topRight, std::string flag = "default")
        : min(bottomLeft.x, topRight.y), max(topRight.x, bottomLeft.y), flag(flag) {}

        /**
         * @brief Get the position of the collider.
         * 
         * @return The position of the collider, which is the same as the minimum vector.
         */
        glm::vec2 getPosition() {
            return min;
        }

        /**
         * @brief Get the size of the collider.
         * 
         * @return The size of the collider, which is the difference between the maximum and minimum vectors.
         */
        glm::vec2 getSize() {
            return max - min;
        }
    };
}