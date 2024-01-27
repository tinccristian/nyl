// ColliderComponent.h
#pragma once
#include "component.h"
#include <glm/glm.hpp>

namespace Nyl
{
    class NYL_API ColliderComponent : public Component {
    public:
        glm::vec2 Position, Size, min, max; ///< Position, Size, min and max vectors of the collider.

        /**
         * @brief Construct a new ColliderComponent object.
         * 
         * @param posX X position of the collider.
         * @param posY Y position of the collider.
         * @param sizeX Width of the collider.
         * @param sizeY Height of the collider.
         */
        ColliderComponent(float posX, float posY, float sizeX, float sizeY)
            : Position(posX, posY), Size(sizeX, sizeY) {
            min = Position - Size / 2.0f;
            max = Position + Size / 2.0f;
        }

        /**
         * @brief Update the position of the collider.
         * 
         * @param position The new position of the collider.
         */
        void Update(const glm::vec2& position) {
            Position = position;
            min = Position - Size / 2.0f;
            max = Position + Size / 2.0f;
        }

        /**
         * @brief Set the size of the collider.
         * 
         * @param size The new size of the collider.
         */
        void SetSize(const glm::vec2& size) {
            Size = size;
            min = Position - Size / 2.0f;
            max = Position + Size / 2.0f;
        }
    };
}