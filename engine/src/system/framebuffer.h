#pragma once

#include "core.h"

namespace nyl
{
    /**
     * @brief Off-screen render target (FBO + colour texture).
     *
     * The editor renders the scene into this and displays colorTexture() inside
     * an ImGui::Image, giving a viewport panel. 2D only, so no depth buffer.
     */
    class NYL_API Framebuffer
    {
    public:
        Framebuffer() = default;
        ~Framebuffer();

        void create(int width, int height);
        void bind();                                  // render into the FBO
        void clear(float r, float g, float b, float a = 1.0f); // clear current target
        void unbind(int screenWidth, int screenHeight); // back to the default FB

        unsigned int colorTexture() const { return m_color; }
        int width() const { return m_width; }
        int height() const { return m_height; }

    private:
        void destroy();

        unsigned int m_fbo = 0;
        unsigned int m_color = 0;
        int m_width = 0;
        int m_height = 0;
    };
}
