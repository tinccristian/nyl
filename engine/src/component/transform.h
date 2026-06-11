#pragma once
#include "component.h"
#include <glm/glm.hpp>

struct TransformComponent : public Component {
public:
    glm::vec2 position; ///< Position of the entity.
    float rotation; ///< Rotation of the entity (in degrees).
    glm::vec2 scale; ///< Scale of the entity.
    glm::vec2 size; ///< Size of the entity.
    float direction=1.0f; ///< Direction of the entity (1 = right, -1 = left).
    glm::vec2 min; ///< Minimum point of the entity (bottom-left corner).
    glm::vec2 max; ///< Maximum point of the entity (top-right corner).

    /**
     * @brief Construct a new TransformComponent object.
     * 
     * @param position Initial position of the entity.
     * @param rotation Initial rotation of the entity (in degrees).
     * @param scale Initial scale of the entity.
     * @param size Initial size of the entity.
     */
    TransformComponent(
        const glm::vec2& position = glm::vec2(0.0f),
        float rotation = 0.0f,
        const glm::vec2& scale = glm::vec2(1.0f),
        const glm::vec2& size = glm::vec2(1.0f))
        : position(position), rotation(rotation), scale(scale), size(size) {
            updateMinMax();
        }

    /**
     * @brief Construct a new TransformComponent object using floats.
     *
     * No default arguments: the vec2 constructor above is the single
     * default-constructible overload, which removes the ambiguity that made
     * a no-argument `TransformComponent{}` fail to compile.
     *
     * @param posX Initial x position of the entity.
     * @param posY Initial y position of the entity.
     * @param rotation Initial rotation of the entity (in degrees).
     * @param scaleX Initial x scale of the entity.
     * @param scaleY Initial y scale of the entity.
     * @param sizeX Initial x size of the entity.
     * @param sizeY Initial y size of the entity.
     */
    TransformComponent(float posX,
        float posY,
        float rotation,
        float scaleX,
        float scaleY,
        float sizeX,
        float sizeY)
        : position(glm::vec2(posX, posY)), rotation(rotation), scale(glm::vec2(scaleX, scaleY)), size(glm::vec2(sizeX, sizeY)) {
            updateMinMax();
        }

    /**
     * @brief Update the min and max points of the entity.
     */
    void updateMinMax() {
        min = position;
        max = position + size;
    }
};