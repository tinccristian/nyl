#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "texture.h"
#include "component.h"

namespace nyl
{
class TilemapComponent: public Component {
public:
    std::vector<std::vector<int>> tiles;
    int tileWidth;
    int tileHeight;
    int mapWidth;
    int mapHeight;
    TextureComponent* tilesetTexture;

    TilemapComponent(int mapWidth, int mapHeight, int tileWidth, int tileHeight, TextureComponent* tilesetTexture)
        : mapWidth(mapWidth), mapHeight(mapHeight), tileWidth(tileWidth), tileHeight(tileHeight), tilesetTexture(tilesetTexture) {
        tiles.resize(mapHeight, std::vector<int>(mapWidth, 0));
    }

    void setTile(int x, int y, int tileId) {
        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
            tiles[y][x] = tileId;
        }
    }

    int getTile(int x, int y) const {
        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
            return tiles[y][x];
        }
        return -1;
    }
};
}