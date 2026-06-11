#include "font_renderer.h"
#include "system_renderer.h"
#include "texture.h"
#include "resource_manager.h"

#include <glad/glad.h>
#include <vector>

#include "font8x8_basic.h" // public-domain 8x8 font: char font8x8_basic[128][8]

namespace nyl
{
    bool FontRenderer::init()
    {
        if (m_atlas) return true;

        const int cols = 16, rows = 8, glyph = 8;
        const int W = cols * glyph; // 128
        const int H = rows * glyph; // 64
        std::vector<unsigned char> data(static_cast<std::size_t>(W) * H * 4, 0);

        for (int c = 0; c < 128; ++c)
        {
            const int gx = (c % cols) * glyph;
            const int gy = (c / cols) * glyph;
            for (int br = 0; br < glyph; ++br)
            {
                const unsigned char bits = static_cast<unsigned char>(font8x8_basic[c][br]);
                for (int b = 0; b < glyph; ++b)
                {
                    if (bits & (1 << b))
                    {
                        const int px = gx + b;
                        const int py = gy + br;
                        const std::size_t idx = (static_cast<std::size_t>(py) * W + px) * 4;
                        data[idx + 0] = 255;
                        data[idx + 1] = 255;
                        data[idx + 2] = 255;
                        data[idx + 3] = 255;
                    }
                }
            }
        }

        TextureComponent* tex = new TextureComponent();
        tex->object_format = GL_RGBA;
        tex->image_format = GL_RGBA;
        tex->Generate(W, H, data.data());
        ResourceManager::Textures["__font8x8"] = tex; // freed by ResourceManager::Clear()
        m_atlas = tex;
        return true;
    }

    void FontRenderer::drawText(RenderSystem& renderer, const std::string& text, glm::vec2 position,
                                float scale, glm::vec4 color, int layer)
    {
        if (!m_atlas) return;

        const float glyph = 8.0f;
        const glm::vec2 uvScale(glyph / m_atlas->width, glyph / m_atlas->height); // (1/16, 1/8)
        glm::vec2 cursor = position;

        for (char ch : text)
        {
            const unsigned char c = static_cast<unsigned char>(ch);
            if (c == '\n')
            {
                cursor.x = position.x;
                cursor.y += glyph * scale;
                continue;
            }
            if (c < 128)
            {
                const int col = c % 16;
                const int row = c / 16;
                const glm::vec2 uvOffset(col * uvScale.x, row * uvScale.y);
                renderer.drawSprite(*m_atlas, cursor, glm::vec2(glyph * scale), 0.0f, color, layer, uvOffset, uvScale);
            }
            cursor.x += glyph * scale;
        }
    }
}
