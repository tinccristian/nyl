#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "system.h"
#include "entity.h"
#include "transform.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "animation.h"

namespace nyl
{
struct Color
{
    glm::vec3 value;
    operator glm::vec3() const { return value; }  // Allow implicit conversion to glm::vec3
};
namespace Colors
{
    static const Color White{ glm::vec3(1.0f, 1.0f, 1.0f) };
    static const Color Black{ glm::vec3(0.0f, 0.0f, 0.0f) };
    static const Color Red{ glm::vec3(1.0f, 0.0f, 0.0f) };
    static const Color Green{ glm::vec3(0.0f, 1.0f, 0.0f) };
    static const Color Blue{ glm::vec3(0.0f, 0.0f, 1.0f) };
    static const Color Yellow{ glm::vec3(1.0f, 1.0f, 0.0f) };
    static const Color Cyan{ glm::vec3(0.0f, 1.0f, 1.0f) };
    static const Color Magenta{ glm::vec3(1.0f, 0.0f, 1.0f) };
    static const Color Orange{ glm::vec3(1.0f, 0.5f, 0.0f) };
    static const Color Purple{ glm::vec3(0.5f, 0.0f, 0.5f) };
    static const Color Pink{ glm::vec3(1.0f, 0.75f, 0.8f) };
    static const Color Gray{ glm::vec3(0.5f, 0.5f, 0.5f) };
    static const Color LightBlue{ glm::vec3(0.5f, 0.5f, 1.0f) };
    static const Color LightGreen{ glm::vec3(0.5f, 1.0f, 0.5f) };
    static const Color LightRed{ glm::vec3(1.0f, 0.5f, 0.5f) };
    static const Color DarkBlue{ glm::vec3(0.0f, 0.0f, 0.5f) };
    static const Color DarkGreen{ glm::vec3(0.0f, 0.5f, 0.0f) };
    static const Color DarkRed{ glm::vec3(0.5f, 0.0f, 0.0f) };
    static const Color LightYellow{ glm::vec3(1.0f, 1.0f, 0.5f) };
    static const Color DarkYellow{ glm::vec3(0.5f, 0.5f, 0.0f) };
}

class NYL_API RenderSystem : public System {
    public:
        // constructor (inits shaders/shapes)
        RenderSystem(ShaderComponent &shader, float screenWidth, float screenHeight);
        // destructor
        ~RenderSystem();
        // update all Entities in this system.
        void update() override;
        // initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
        // draw a defined quad textured with given sprite
        void DrawSprite(const TextureComponent& texture, glm::vec2 position = glm::vec2(0.0f,0.0f), glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
        //void DrawObject(const TextureComponent& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f), float direction = 0.0f);

        // draw functions
        void DrawEntity(const Entity& entity, float deltaTime);
        void DrawRectangleOutline(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color);
        
        // helper functions
        bool CheckRenderComponents(const Entity& entity);
        glm::vec2 getGlfwCoordinates(glm::vec2 worldPos, glm::vec2 windowSize);
        glm::vec2 GetWorldPosition(const Entity& entity);


    private:
        // render state
        ShaderComponent shader;
        glm::vec2 windowSize;
        unsigned int quadVAO;
        unsigned int outlineVAO;
        void CheckGLError(const char* operation);
    };
}