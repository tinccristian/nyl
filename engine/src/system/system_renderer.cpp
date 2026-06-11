#include "system_renderer.h"
#include "resource_manager.h"
#include "animation.h"
#include "transform.h"
#include "log.h"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>

using namespace nyl;

RenderSystem::RenderSystem(float screenWidth, float screenHeight)
    : windowSize(glm::vec2(screenWidth, screenHeight))
{
    shader = ResourceManager::GetShader("batch");
    if (!shader)
        NYL_CORE_ERROR("RenderSystem: 'batch' shader not loaded");

    // top-left origin, y-down world space
    projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)offsetof(QuadVertex, color));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

RenderSystem::~RenderSystem()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void RenderSystem::beginFrame(glm::vec2 cameraPos, float zoom)
{
    if (zoom <= 0.0f) zoom = 1.0f;
    // zoom about the viewport centre, then offset by the camera position
    glm::vec2 c = windowSize * 0.5f;
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(c, 0.0f));
    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
    view = glm::translate(view, glm::vec3(-c, 0.0f));
    view = glm::translate(view, glm::vec3(-cameraPos, 0.0f));
    quads.clear();
}

void RenderSystem::beginFrame(const Camera& camera)
{
    beginFrame(camera.position, camera.zoom);
}

void RenderSystem::endFrame()
{
    flush();
}

void RenderSystem::submitQuad(unsigned int texture, glm::vec2 pos, glm::vec2 size, float rotation,
                              glm::vec4 color, int layer, glm::vec2 uvOffset, glm::vec2 uvScale, bool flipX)
{
    // local corners matching a 2-triangle quad (matches the previous winding)
    static const glm::vec2 corners[6] = {
        {0,1},{1,0},{0,0},
        {0,1},{1,1},{1,0}
    };

    Quad q;
    q.layer = layer;
    q.texture = texture;

    const glm::vec2 center = pos + size * 0.5f;
    const float rad = glm::radians(rotation);
    const float cs = std::cos(rad);
    const float sn = std::sin(rad);

    for (int i = 0; i < 6; ++i)
    {
        const glm::vec2 lc = corners[i];
        glm::vec2 world = pos + lc * size;
        glm::vec2 d = world - center;
        glm::vec2 r = { d.x * cs - d.y * sn, d.x * sn + d.y * cs };

        q.v[i].pos = center + r;
        float u = flipX ? (1.0f - lc.x) : lc.x;
        q.v[i].uv = uvOffset + glm::vec2(u, lc.y) * uvScale;
        q.v[i].color = color;
    }

    quads.push_back(q);
}

void RenderSystem::drawSprite(const TextureComponent& texture, glm::vec2 position, glm::vec2 size,
                              float rotation, glm::vec3 color, int layer, glm::vec2 uvOffset, glm::vec2 uvScale)
{
    submitQuad(texture.ID, position, size, rotation, glm::vec4(color, 1.0f), layer, uvOffset, uvScale, false);
}

void RenderSystem::drawSprite(const TextureComponent& texture, glm::vec2 position, glm::vec2 size,
                              float rotation, glm::vec4 color, int layer, glm::vec2 uvOffset, glm::vec2 uvScale)
{
    submitQuad(texture.ID, position, size, rotation, color, layer, uvOffset, uvScale, false);
}

void RenderSystem::drawEntity(const Entity& entity, float deltaTime, int layer)
{
    auto transform = entity.getComponent<TransformComponent>();
    if (!transform) { return; }

    const TextureComponent* texture = nullptr;
    Animation* currentAnimation = nullptr;

    if (auto animated = entity.getComponent<AnimatedComponent>())
    {
        animated->Update(deltaTime);
        currentAnimation = animated->GetCurrentAnimation();
        texture = animated->GetCurrentTexture();
    }
    else if (auto tex = entity.getComponent<TextureComponent>())
    {
        texture = tex;
    }

    if (!texture)
    {
        NYL_CORE_ERROR("RenderSystem::drawEntity: entity has no valid texture");
        return;
    }

    glm::vec2 uvOffset(0.0f);
    glm::vec2 uvScale(1.0f);
    if (currentAnimation && texture->width > 0)
    {
        float frameWidth = static_cast<float>(currentAnimation->frameWidth) / texture->width;
        uvOffset = glm::vec2(frameWidth * currentAnimation->currentFrame, 0.0f);
        uvScale = glm::vec2(frameWidth, 1.0f);
    }

    const bool flipX = transform->direction < 0.0f;
    submitQuad(texture->ID, transform->position, transform->size, transform->rotation,
               glm::vec4(1.0f), layer, uvOffset, uvScale, flipX);
}

void RenderSystem::flush()
{
    if (quads.empty() || !shader) { quads.clear(); return; }

    std::sort(quads.begin(), quads.end(), [](const Quad& a, const Quad& b) {
        if (a.layer != b.layer) return a.layer < b.layer;
        return a.texture < b.texture;
    });

    shader->use();
    shader->set_mat4("projection", projection);
    shader->set_mat4("view", view);
    shader->set_int("uTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    std::vector<QuadVertex> batch;
    batch.reserve(quads.size() * 6);
    unsigned int curTex = quads.front().texture;

    auto emit = [&]() {
        if (batch.empty()) return;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(batch.size() * sizeof(QuadVertex)),
                     batch.data(), GL_DYNAMIC_DRAW);
        glBindTexture(GL_TEXTURE_2D, curTex);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(batch.size()));
        batch.clear();
    };

    for (const Quad& q : quads)
    {
        if (q.texture != curTex) { emit(); curTex = q.texture; }
        for (int i = 0; i < 6; ++i) batch.push_back(q.v[i]);
    }
    emit();

    glBindVertexArray(0);
    checkGLError("RenderSystem::flush");
    quads.clear();
}

void RenderSystem::checkGLError(const char* operation)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        NYL_CORE_ERROR("OpenGL error after {0}: {1}", operation, error);
    }
}
