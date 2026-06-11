#include "system_particle.h"
#include "system_renderer.h"
#include "resource_manager.h"
#include "particle.h"
#include "transform.h"

#include <random>
#include <algorithm>
#include <glm/glm.hpp>

namespace nyl
{
    static float randRange(float lo, float hi)
    {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(lo, hi);
        return dist(rng);
    }

    void ParticleSystem::update(Scene& scene, float deltaTime)
    {
        scene.forEach<TransformComponent, ParticleEmitterComponent>(
            [&](EntityID, TransformComponent& transform, ParticleEmitterComponent& emitter)
            {
                // spawn
                if (emitter.emitting && emitter.emissionRate > 0.0f)
                {
                    emitter.emitAccumulator += emitter.emissionRate * deltaTime;
                    while (emitter.emitAccumulator >= 1.0f &&
                           static_cast<int>(emitter.particles.size()) < emitter.maxParticles)
                    {
                        emitter.emitAccumulator -= 1.0f;
                        Particle p;
                        p.position = transform.position;
                        p.velocity = glm::vec2(
                            randRange(emitter.velocityMin.x, emitter.velocityMax.x),
                            randRange(emitter.velocityMin.y, emitter.velocityMax.y));
                        p.maxLife = emitter.particleLife;
                        p.life = emitter.particleLife;
                        emitter.particles.push_back(p);
                    }
                }

                // integrate + age
                for (Particle& p : emitter.particles)
                {
                    p.velocity += emitter.gravity * deltaTime;
                    p.position += p.velocity * deltaTime;
                    p.life -= deltaTime;
                }

                // remove dead
                auto& ps = emitter.particles;
                ps.erase(std::remove_if(ps.begin(), ps.end(),
                            [](const Particle& p) { return p.life <= 0.0f; }),
                         ps.end());
            });
    }

    void ParticleSystem::render(Scene& scene, RenderSystem& renderer, int layer)
    {
        TextureComponent* white = ResourceManager::GetWhiteTexture();
        if (!white) return;

        scene.forEach<ParticleEmitterComponent>(
            [&](EntityID, ParticleEmitterComponent& emitter)
            {
                for (const Particle& p : emitter.particles)
                {
                    float t = (p.maxLife > 0.0f) ? (1.0f - p.life / p.maxLife) : 1.0f;
                    float size = glm::mix(emitter.startSize, emitter.endSize, t);
                    glm::vec4 color = glm::mix(emitter.startColor, emitter.endColor, t);
                    glm::vec2 half(size * 0.5f);
                    renderer.drawSprite(*white, p.position - half, glm::vec2(size), 0.0f, color, layer);
                }
            });
    }
}
