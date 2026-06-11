#pragma once

#include <vector>
#include <string>

#include "core.h"
#include "component.h"

namespace nyl
{
    /**
     * @brief A grid of tiles indexing into a tileset texture.
     *
     * Tile values are 1-based (matching Tiled's GIDs); 0 means "empty". The map
     * is positioned by its entity's TransformComponent. Rendered by
     * TilemapSystem through the batch renderer (one draw call per tileset).
     */
    class NYL_API TilemapComponent : public Component
    {
    public:
        int   width = 0;            // in tiles
        int   height = 0;           // in tiles
        float tileWidth = 32.0f;    // pixels
        float tileHeight = 32.0f;   // pixels
        int   tilesetColumns = 1;   // columns in the tileset texture
        std::string tilesetTexture; // ResourceManager texture name
        std::vector<int> tiles;     // width*height, 0 = empty, else 1-based id
        bool  collidable = false;   // generate colliders for non-empty tiles

        TilemapComponent() = default;

        int tileAt(int x, int y) const
        {
            if (x < 0 || y < 0 || x >= width || y >= height) return 0;
            return tiles[static_cast<std::size_t>(y) * width + x];
        }
    };
}
