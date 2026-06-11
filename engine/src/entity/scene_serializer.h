#pragma once

#include <string>

#include "core.h"

namespace nyl
{
    class Scene;

    /**
     * @brief Saves/loads a Scene to/from JSON (.scene files).
     *
     * Serialization is registry-driven: every component type registered with a
     * serialize/deserialize pair (see ComponentRegistry) is round-tripped, so
     * game-defined components participate automatically. This is the data layer
     * the ImGui editor (Phase 8) builds on.
     */
    class NYL_API SceneSerializer
    {
    public:
        static std::string ToJsonString(Scene& scene, int indent = 2);
        static bool        FromJsonString(Scene& scene, const std::string& jsonText);

        static bool Save(Scene& scene, const std::string& filepath);
        static bool Load(Scene& scene, const std::string& filepath);
    };
}
