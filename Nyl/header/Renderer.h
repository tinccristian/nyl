#pragma once
// nyl
#include "Core.h"
#include "Texture.h"
#include "Shader.h"
// gl
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// std
#include <map>

#include "Shader.h"
#include "Texture.h"

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
        // Add more colors here...
    }

    class NYL_API SpriteRenderer
    {
    public:
        // constructor (inits shaders/shapes)
        SpriteRenderer(Shader &shader);
        // destructor
        ~SpriteRenderer();
        // renders a defined quad textured with given sprite
        void DrawSprite(const Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
        void DrawRectangleOutline(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
    private:
        // render state
        Shader       shader;
        unsigned int quadVAO;
        unsigned int outlineVAO;
        // initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
    };
}