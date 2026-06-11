#include "scene_serializer.h"

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include "scene.h"
#include "component_registry.h"
#include "log.h"

namespace nyl
{
    using json = nlohmann::json;

    std::string SceneSerializer::ToJsonString(Scene& scene, int indent)
    {
        json root;
        root["version"] = 1;

        json entitiesJson = json::array();
        for (EntityID id : scene.allEntities())
        {
            json eJson;
            eJson["id"] = id.index;

            json comps = json::object();
            for (const ComponentInfo* info : ComponentRegistry::All())
            {
                if (info->serialize && info->has(scene, id))
                {
                    json cj;
                    info->serialize(scene, id, cj);
                    comps[info->name] = cj;
                }
            }
            eJson["components"] = comps;
            entitiesJson.push_back(eJson);
        }
        root["entities"] = entitiesJson;

        return root.dump(indent);
    }

    bool SceneSerializer::FromJsonString(Scene& scene, const std::string& jsonText)
    {
        json root = json::parse(jsonText, nullptr, /*allow_exceptions*/ false);
        if (root.is_discarded())
        {
            NYL_CORE_ERROR("SceneSerializer: failed to parse JSON");
            return false;
        }

        scene.clear();
        if (!root.contains("entities")) return true;

        for (const json& eJson : root["entities"])
        {
            EntityID id = scene.createEntityID();
            if (!eJson.contains("components")) continue;
            for (auto it = eJson["components"].begin(); it != eJson["components"].end(); ++it)
            {
                const ComponentInfo* info = ComponentRegistry::FindByName(it.key());
                if (info && info->deserialize)
                    info->deserialize(scene, id, it.value());
            }
        }
        return true;
    }

    bool SceneSerializer::Save(Scene& scene, const std::string& filepath)
    {
        std::ofstream out(filepath);
        if (!out)
        {
            NYL_CORE_ERROR("SceneSerializer: cannot open '{}' for writing", filepath);
            return false;
        }
        out << ToJsonString(scene);
        NYL_CORE_INFO("Scene saved to {}", filepath);
        return true;
    }

    bool SceneSerializer::Load(Scene& scene, const std::string& filepath)
    {
        std::ifstream in(filepath);
        if (!in)
        {
            NYL_CORE_ERROR("SceneSerializer: cannot open '{}' for reading", filepath);
            return false;
        }
        std::stringstream ss;
        ss << in.rdbuf();
        bool ok = FromJsonString(scene, ss.str());
        if (ok) NYL_CORE_INFO("Scene loaded from {}", filepath);
        return ok;
    }
}
