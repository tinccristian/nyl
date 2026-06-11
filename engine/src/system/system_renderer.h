#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "system.h"
#include "entity.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

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
}

/**
 * @brief Batched 2D sprite renderer with a single unified world-space camera.
 *
 * Submit sprites/entities between beginFrame() and endFrame(); quads are
 * collected, sorted by (layer, texture) and flushed as a minimal number of
 * draw calls (one per contiguous texture run). All draws — backgrounds,
 * sprites, tiles, entities — go through the same view/projection, so there is
 * a single coordinate space (world units, top-left origin, y-down).
 */
class NYL_API RenderSystem : public System {
public:
    RenderSystem(float screenWidth, float screenHeight);
    ~RenderSystem() override;

    // ---- frame API ----
    void beginFrame(glm::vec2 cameraPos = glm::vec2(0.0f), float zoom = 1.0f);
    void beginFrame(const Camera& camera);
    void endFrame();

    // ---- submission ----
    void drawSprite(const TextureComponent& texture,
                    glm::vec2 position,
                    glm::vec2 size,
                    float rotation = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f),
                    int layer = 0,
                    glm::vec2 uvOffset = glm::vec2(0.0f),
                    glm::vec2 uvScale = glm::vec2(1.0f));

    // alpha-capable variant (particles, fades)
    void drawSprite(const TextureComponent& texture,
                    glm::vec2 position,
                    glm::vec2 size,
                    float rotation,
                    glm::vec4 color,
                    int layer = 0,
                    glm::vec2 uvOffset = glm::vec2(0.0f),
                    glm::vec2 uvScale = glm::vec2(1.0f));

    void drawEntity(const Entity& entity, float deltaTime, int layer = 0);

    glm::vec2 windowSize;

private:
    struct QuadVertex { glm::vec2 pos; glm::vec2 uv; glm::vec4 color; };
    struct Quad { int layer; unsigned int texture; QuadVertex v[6]; };

    void submitQuad(unsigned int texture, glm::vec2 pos, glm::vec2 size, float rotation,
                    glm::vec4 color, int layer, glm::vec2 uvOffset, glm::vec2 uvScale, bool flipX);
    void flush();
    void checkGLError(const char* operation);

    ShaderComponent* shader = nullptr; // owned by ResourceManager
    glm::mat4 projection{ 1.0f };
    glm::mat4 view{ 1.0f };
    unsigned int vao = 0;
    unsigned int vbo = 0;
    std::vector<Quad> quads;
};
}
