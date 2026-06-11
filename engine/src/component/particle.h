#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "core.h"
#include "component.h"

namespace nyl
{
    struct Particle
    {
        glm::vec2 position{ 0.0f };
        glm::vec2 velocity{ 0.0f };
        float     life = 0.0f;     // remaining seconds
        float     maxLife = 1.0f;
    };

    /**
     * @brief A CPU particle emitter attached to an entity (emits at its
     * transform position). Updated by ParticleSystem, drawn through the batch.
     */
    class NYL_API ParticleEmitterComponent : public Component
    {
    public:
        // configuration
        bool      emitting = true;
        float     emissionRate = 30.0f;        // particles per second
        float     particleLife = 1.2f;         // seconds
        int       maxParticles = 300;
        glm::vec2 gravity{ 0.0f, 40.0f };
        glm::vec2 velocityMin{ -25.0f, -70.0f };
        glm::vec2 velocityMax{ 25.0f, -30.0f };
        float     startSize = 8.0f;
        float     endSize = 0.0f;
        glm::vec4 startColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec4 endColor{ 1.0f, 1.0f, 1.0f, 0.0f };

        // runtime state
        std::vector<Particle> particles;
        float emitAccumulator = 0.0f;

        ParticleEmitterComponent() = default;
    };
}
