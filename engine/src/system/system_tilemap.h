#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "system.h"
#include "scene.h"
#include "tilemap.h"
#include "collider.h"

namespace nyl
{
    class RenderSystem;

    /**
     * @brief Renders TilemapComponents and imports/derives data for them.
     */
    class NYL_API TilemapSystem : public System
    {
    public:
        /// Draw every entity's tilemap through the batch renderer.
        void render(Scene& scene, RenderSystem& renderer, int layer = 0);

        /// Parse a Tiled (https://mapeditor.org) JSON map's first tile layer.
        /// The caller still loads + assigns the tileset texture name.
        static bool LoadTiledJson(const std::string& jsonText, TilemapComponent& out);
        static bool LoadTiledFile(const std::string& path, TilemapComponent& out);

        /// Build AABB colliders for every non-empty tile (for the broadphase).
        static void GenerateColliders(const TilemapComponent& map, glm::vec2 origin,
                                      std::vector<BoxCollider>& outColliders);
    };
}
