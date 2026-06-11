#include "system_tilemap.h"
#include "system_renderer.h"
#include "resource_manager.h"
#include "transform.h"
#include "log.h"

#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

namespace nyl
{
    using json = nlohmann::json;

    void TilemapSystem::render(Scene& scene, RenderSystem& renderer, int layer)
    {
        scene.forEach<TransformComponent, TilemapComponent>(
            [&](EntityID, TransformComponent& transform, TilemapComponent& map)
            {
                if (map.tiles.empty() || map.tilesetColumns <= 0) return;
                TextureComponent* tex = ResourceManager::GetTexture(map.tilesetTexture);
                if (!tex || tex->width == 0 || tex->height == 0) return;

                const glm::vec2 uvScale(map.tileWidth / tex->width, map.tileHeight / tex->height);

                for (int y = 0; y < map.height; ++y)
                {
                    for (int x = 0; x < map.width; ++x)
                    {
                        int id = map.tileAt(x, y);
                        if (id <= 0) continue;
                        int index = id - 1;
                        int col = index % map.tilesetColumns;
                        int row = index / map.tilesetColumns;

                        glm::vec2 uvOffset(col * uvScale.x, row * uvScale.y);
                        glm::vec2 pos = transform.position + glm::vec2(x * map.tileWidth, y * map.tileHeight);
                        renderer.drawSprite(*tex, pos, glm::vec2(map.tileWidth, map.tileHeight),
                                            0.0f, glm::vec3(1.0f), layer, uvOffset, uvScale);
                    }
                }
            });
    }

    bool TilemapSystem::LoadTiledJson(const std::string& jsonText, TilemapComponent& out)
    {
        json root = json::parse(jsonText, nullptr, false);
        if (root.is_discarded())
        {
            NYL_CORE_ERROR("TilemapSystem: failed to parse Tiled JSON");
            return false;
        }

        out.width      = root.value("width", 0);
        out.height     = root.value("height", 0);
        out.tileWidth  = static_cast<float>(root.value("tilewidth", 32));
        out.tileHeight = static_cast<float>(root.value("tileheight", 32));

        // first embedded tileset (if any) gives us the column count
        if (root.contains("tilesets") && root["tilesets"].is_array() && !root["tilesets"].empty())
        {
            const json& ts = root["tilesets"][0];
            out.tilesetColumns = ts.value("columns", out.tilesetColumns);
        }

        // first tile layer's data
        if (root.contains("layers") && root["layers"].is_array())
        {
            for (const json& layer : root["layers"])
            {
                if (layer.value("type", std::string()) == "tilelayer" && layer.contains("data"))
                {
                    out.tiles.clear();
                    for (const json& gid : layer["data"])
                        out.tiles.push_back(gid.get<int>());
                    break;
                }
            }
        }

        if (out.tilesetColumns <= 0) out.tilesetColumns = 1;
        return !out.tiles.empty();
    }

    bool TilemapSystem::LoadTiledFile(const std::string& path, TilemapComponent& out)
    {
        std::ifstream in(path);
        if (!in)
        {
            NYL_CORE_ERROR("TilemapSystem: cannot open Tiled file '{}'", path);
            return false;
        }
        std::stringstream ss;
        ss << in.rdbuf();
        return LoadTiledJson(ss.str(), out);
    }

    void TilemapSystem::GenerateColliders(const TilemapComponent& map, glm::vec2 origin,
                                          std::vector<BoxCollider>& outColliders)
    {
        for (int y = 0; y < map.height; ++y)
        {
            for (int x = 0; x < map.width; ++x)
            {
                if (map.tileAt(x, y) <= 0) continue;
                glm::vec2 min = origin + glm::vec2(x * map.tileWidth, y * map.tileHeight);
                glm::vec2 max = min + glm::vec2(map.tileWidth, map.tileHeight);
                outColliders.emplace_back(min, max, "tile");
            }
        }
    }
}
