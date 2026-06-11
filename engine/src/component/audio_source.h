#pragma once

#include <string>

#include "core.h"
#include "component.h"

namespace nyl
{
    /**
     * @brief Declares a sound clip that belongs to an entity (editor/serialization
     * friendly). Playback is performed through AudioEngine.
     */
    class NYL_API AudioSourceComponent : public Component
    {
    public:
        std::string clip;          // file path (relative to the resource root)
        bool        autoplay = false;
        bool        loop = false;
        float       volume = 1.0f;

        AudioSourceComponent() = default;
    };
}
