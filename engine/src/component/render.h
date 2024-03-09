#pragma once

#include "component.h"
#include "texture.h"
#include <glm/glm.hpp>

namespace nyl{
    class RenderComponent : public Component {
    public:
        RenderComponent(const TextureComponent& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
            : texture(texture), position(position), size(size), rotate(rotate), color(color) {}
        TextureComponent texture; // The texture of the entity
        glm::vec2 position; // The position of the entity
        glm::vec2 size; // The size of the entity
        float rotate; // The rotation of the entity
        glm::vec3 color; // The color of the entity
    };
}