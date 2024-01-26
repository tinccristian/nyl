#pragma once

#include "system.h"
#include "component_render.h"
#include "component_shader.h"
#include "component_texture.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Nyl
{
    struct Color
    {
        glm::vec3 value;
    };
    namespace Colors
    {
        static const Color Red{ glm::vec3(1.0f, 0.0f, 0.0f) };
        static const Color Green{ glm::vec3(0.0f, 1.0f, 0.0f) };
        static const Color Blue{ glm::vec3(0.0f, 0.0f, 1.0f) };
        static const Color Yellow{ glm::vec3(1.0f, 1.0f, 0.0f) };
        static const Color Cyan{ glm::vec3(0.0f, 1.0f, 1.0f) };
        static const Color Magenta{ glm::vec3(1.0f, 0.0f, 1.0f) };
        static const Color Orange{ glm::vec3(1.0f, 0.5f, 0.0f) };
        static const Color Purple{ glm::vec3(0.5f, 0.0f, 0.5f) };
        static const Color Pink{ glm::vec3(1.0f, 0.75f, 0.8f) };
        // Add more colors here...
        static const Color Color1{ glm::vec3(0.1f, 0.2f, 0.3f) };
        static const Color Color2{ glm::vec3(0.4f, 0.5f, 0.6f) };
        static const Color Color3{ glm::vec3(0.7f, 0.8f, 0.9f) };
        // Add more colors here...
        static const Color Color48{ glm::vec3(0.1f, 0.2f, 0.3f) };
        static const Color Color49{ glm::vec3(0.4f, 0.5f, 0.6f) };
        static const Color Color50{ glm::vec3(0.7f, 0.8f, 0.9f) };
    }
    class NYL_API RenderSystem : public System {
    public:
        // constructor (inits shaders/shapes)
        RenderSystem(ShaderComponent &shader);
        // destructor
        ~RenderSystem();
        // Update all Entities in this system.
        void update() override;
        // initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
        // Draw a defined quad textured with given sprite
        void DrawSprite(const TextureComponent& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
        void DrawObject(const TextureComponent& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f), float direction = 0.0f);
        void DrawRectangleOutline(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
    private:
        // render state
        ShaderComponent shader;
        unsigned int quadVAO;
        unsigned int outlineVAO;
        void CheckGLError();
    };
}