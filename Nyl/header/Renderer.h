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

    class NYL_API SpriteRenderer
    {
    public:
        // constructor (inits shaders/shapes)
        SpriteRenderer(Shader &shader);
        // destructor
        ~SpriteRenderer();
        // renders a defined quad textured with given sprite
        void DrawSprite(const Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    private:
        // render state
        Shader       shader;
        unsigned int quadVAO;
        // initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
    };
}