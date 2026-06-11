#pragma once

#include "system.h"
#include "scene.h"

namespace nyl
{
    class RenderSystem;

    /**
     * @brief Updates and renders every entity's ParticleEmitterComponent.
     *
     * update() spawns/integrates/ages particles (call from FixedUpdate or
     * Update); render() submits them to the batch renderer as faded quads.
     */
    class NYL_API ParticleSystem : public System
    {
    public:
        void update(Scene& scene, float deltaTime);
        void render(Scene& scene, RenderSystem& renderer, int layer = 0);
    };
}
