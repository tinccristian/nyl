// System.h
#pragma once

#include "core.h"

namespace nyl
{
    /**
     * @class System
     * @brief Minimal base class for engine systems.
     *
     * Systems no longer store copies of entities (that was the source of the
     * old id-recycling / dangling-state bug). Instead each system operates over
     * a Scene, e.g. `PhysicsSystem::update(Scene&, float)`. This base exists
     * only to give systems a common, polymorphic anchor.
     */
    class NYL_API System
    {
    public:
        System() = default;
        virtual ~System() = default;
    };
}
