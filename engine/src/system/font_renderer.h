#pragma once

#include <string>
#include <glm/glm.hpp>

#include "core.h"

namespace nyl
{
    class RenderSystem;
    class TextureComponent;

    /**
     * @brief Minimal bitmap text renderer.
     *
     * Builds a texture atlas from an embedded public-domain 8x8 font and draws
     * strings as batched quads (so text shares draw calls with sprites). No
     * external font file needed. drawText() must be called inside a renderer
     * frame (between beginFrame()/endFrame()).
     */
    class NYL_API FontRenderer
    {
    public:
        bool init(); // builds the atlas; requires an active GL context

        void drawText(RenderSystem& renderer, const std::string& text, glm::vec2 position,
                      float scale = 2.0f, glm::vec4 color = glm::vec4(1.0f), int layer = 1000);

        float glyphSize(float scale) const { return 8.0f * scale; }

    private:
        TextureComponent* m_atlas = nullptr; // owned by ResourceManager
    };
}
