#pragma once

#include <string>
#include <glm/glm.hpp>

#include "core.h"
#include "component.h"

namespace nyl
{
    /**
     * @brief Renders a single texture (referenced by ResourceManager name) at
     * the entity's transform. The editor-friendly way to make an entity visible:
     * pick a texture, tint it, choose a draw layer.
     *
     * Resolution order in RenderSystem::drawEntity is: Animation > Sprite > raw
     * TextureComponent.
     */
    class NYL_API SpriteComponent : public Component
    {
    public:
        std::string texture;          // ResourceManager texture name
        glm::vec4   tint{ 1.0f };     // multiplied with the texture
        int         layer = 0;        // draw order

        SpriteComponent() = default;
    };
}
