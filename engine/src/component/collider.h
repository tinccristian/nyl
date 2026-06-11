// BoxColliderComponent.h
#pragma once
#include "component.h"
#include <glm/glm.hpp>
#include <string>

namespace nyl
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
         * @brief Default constructor (needed for component-pool storage).
         */
        BoxCollider() : min(0.0f), max(0.0f), flag("default") {}

        /**
         * @brief Construct a new BoxCollider object from two corner points.
         *
         * The corners may be given in any order: the collider always stores a
         * true axis-aligned bounding box, where `min` is the component-wise
         * minimum and `max` is the component-wise maximum of the two corners.
         *
         * @param cornerA One corner of the box.
         * @param cornerB The opposite corner of the box.
         * @param flag The flag of the collider.
         */
        BoxCollider(const glm::vec2& cornerA, const glm::vec2& cornerB, std::string flag = "default")
        : min(glm::min(cornerA, cornerB)), max(glm::max(cornerA, cornerB)), flag(flag) {}

        /**
         * @brief Get the minimum (bottom-left) corner of the collider.
         */
        glm::vec2 getMin() const { return min; }

        /**
         * @brief Get the maximum (top-right) corner of the collider.
         */
        glm::vec2 getMax() const { return max; }

        /**
         * @brief Get the position of the collider (its minimum corner).
         */
        glm::vec2 getPosition() const {
            return min;
        }

        /**
         * @brief Get the size of the collider (max - min).
         */
        glm::vec2 getSize() const {
            return max - min;
        }
    };
}