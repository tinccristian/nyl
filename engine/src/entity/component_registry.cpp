#include "component_registry.h"

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <cstdio>

#include "transform.h"
#include "physics.h"
#include "collider.h"
#include "camera.h"
#include "animation.h"
#include "particle.h"
#include "tilemap.h"
#include "audio_source.h"

namespace nyl
{
    using json = nlohmann::json;

    // ---- small glm <-> json helpers ----
    static json vec2ToJson(const glm::vec2& v) { return json::array({ v.x, v.y }); }

    static glm::vec2 jsonToVec2(const json& j, glm::vec2 fallback = glm::vec2(0.0f))
    {
        if (j.is_array() && j.size() == 2)
            return glm::vec2(j[0].get<float>(), j[1].get<float>());
        return fallback;
    }

    static json vec4ToJson(const glm::vec4& v) { return json::array({ v.x, v.y, v.z, v.w }); }

    static glm::vec4 jsonToVec4(const json& j, glm::vec4 fallback = glm::vec4(1.0f))
    {
        if (j.is_array() && j.size() == 4)
            return glm::vec4(j[0].get<float>(), j[1].get<float>(), j[2].get<float>(), j[3].get<float>());
        return fallback;
    }

    // ---- Transform ----
    static void serializeTransform(Scene& s, EntityID e, json& j)
    {
        auto* t = s.getComponent<TransformComponent>(e);
        j["position"]  = vec2ToJson(t->position);
        j["rotation"]  = t->rotation;
        j["scale"]     = vec2ToJson(t->scale);
        j["size"]      = vec2ToJson(t->size);
        j["direction"] = t->direction;
    }
    static void deserializeTransform(Scene& s, EntityID e, const json& j)
    {
        auto& t = s.addComponent<TransformComponent>(e);
        if (j.contains("position"))  t.position  = jsonToVec2(j["position"]);
        if (j.contains("rotation"))  t.rotation  = j["rotation"].get<float>();
        if (j.contains("scale"))     t.scale     = jsonToVec2(j["scale"], glm::vec2(1.0f));
        if (j.contains("size"))      t.size      = jsonToVec2(j["size"], glm::vec2(1.0f));
        if (j.contains("direction")) t.direction = j["direction"].get<float>();
        t.updateMinMax();
    }

    // ---- Physics ----
    static void serializePhysics(Scene& s, EntityID e, json& j)
    {
        auto* p = s.getComponent<PhysicsComponent>(e);
        j["velocity"]  = vec2ToJson(p->velocity);
        j["mass"]      = p->mass;
        j["canJump"]   = p->canJump;
        j["direction"] = p->direction;
    }
    static void deserializePhysics(Scene& s, EntityID e, const json& j)
    {
        auto& p = s.addComponent<PhysicsComponent>(e);
        if (j.contains("velocity"))  p.velocity  = jsonToVec2(j["velocity"]);
        if (j.contains("mass"))      p.mass      = j["mass"].get<float>();
        if (j.contains("canJump"))   p.canJump   = j["canJump"].get<bool>();
        if (j.contains("direction")) p.direction = j["direction"].get<float>();
    }

    // ---- BoxCollider ----
    static void serializeCollider(Scene& s, EntityID e, json& j)
    {
        auto* c = s.getComponent<BoxCollider>(e);
        j["min"]  = vec2ToJson(c->min);
        j["max"]  = vec2ToJson(c->max);
        j["flag"] = c->flag;
    }
    static void deserializeCollider(Scene& s, EntityID e, const json& j)
    {
        auto& c = s.addComponent<BoxCollider>(e);
        if (j.contains("min")) c.min = jsonToVec2(j["min"]);
        if (j.contains("max")) c.max = jsonToVec2(j["max"]);
        if (j.contains("flag")) c.flag = j["flag"].get<std::string>();
    }

    // ---- Camera ----
    static void serializeCamera(Scene& s, EntityID e, json& j)
    {
        auto* c = s.getComponent<Camera>(e);
        j["x"] = c->x; j["y"] = c->y;
        j["width"] = c->width; j["height"] = c->height;
        j["zoom"] = c->zoom;
        j["position"] = vec2ToJson(c->position);
    }
    static void deserializeCamera(Scene& s, EntityID e, const json& j)
    {
        auto& c = s.addComponent<Camera>(e);
        if (j.contains("x")) c.x = j["x"].get<float>();
        if (j.contains("y")) c.y = j["y"].get<float>();
        if (j.contains("width"))  c.width  = j["width"].get<float>();
        if (j.contains("height")) c.height = j["height"].get<float>();
        if (j.contains("zoom"))   c.zoom   = j["zoom"].get<float>();
        if (j.contains("position")) c.position = jsonToVec2(j["position"]);
    }

    // ---- ParticleEmitter ----
    static void serializeParticle(Scene& s, EntityID e, json& j)
    {
        auto* p = s.getComponent<ParticleEmitterComponent>(e);
        j["emitting"]     = p->emitting;
        j["emissionRate"] = p->emissionRate;
        j["particleLife"] = p->particleLife;
        j["maxParticles"] = p->maxParticles;
        j["gravity"]      = vec2ToJson(p->gravity);
        j["velocityMin"]  = vec2ToJson(p->velocityMin);
        j["velocityMax"]  = vec2ToJson(p->velocityMax);
        j["startSize"]    = p->startSize;
        j["endSize"]      = p->endSize;
        j["startColor"]   = vec4ToJson(p->startColor);
        j["endColor"]     = vec4ToJson(p->endColor);
    }
    static void deserializeParticle(Scene& s, EntityID e, const json& j)
    {
        auto& p = s.addComponent<ParticleEmitterComponent>(e);
        if (j.contains("emitting"))     p.emitting     = j["emitting"].get<bool>();
        if (j.contains("emissionRate")) p.emissionRate = j["emissionRate"].get<float>();
        if (j.contains("particleLife")) p.particleLife = j["particleLife"].get<float>();
        if (j.contains("maxParticles")) p.maxParticles = j["maxParticles"].get<int>();
        if (j.contains("gravity"))      p.gravity      = jsonToVec2(j["gravity"]);
        if (j.contains("velocityMin"))  p.velocityMin  = jsonToVec2(j["velocityMin"]);
        if (j.contains("velocityMax"))  p.velocityMax  = jsonToVec2(j["velocityMax"]);
        if (j.contains("startSize"))    p.startSize    = j["startSize"].get<float>();
        if (j.contains("endSize"))      p.endSize      = j["endSize"].get<float>();
        if (j.contains("startColor"))   p.startColor   = jsonToVec4(j["startColor"]);
        if (j.contains("endColor"))     p.endColor     = jsonToVec4(j["endColor"]);
    }

    // ---- Tilemap ----
    static void serializeTilemap(Scene& s, EntityID e, json& j)
    {
        auto* m = s.getComponent<TilemapComponent>(e);
        j["width"]          = m->width;
        j["height"]         = m->height;
        j["tileWidth"]      = m->tileWidth;
        j["tileHeight"]     = m->tileHeight;
        j["tilesetColumns"] = m->tilesetColumns;
        j["tilesetTexture"] = m->tilesetTexture;
        j["collidable"]     = m->collidable;
        j["tiles"]          = m->tiles;
    }
    static void deserializeTilemap(Scene& s, EntityID e, const json& j)
    {
        auto& m = s.addComponent<TilemapComponent>(e);
        if (j.contains("width"))          m.width          = j["width"].get<int>();
        if (j.contains("height"))         m.height         = j["height"].get<int>();
        if (j.contains("tileWidth"))      m.tileWidth      = j["tileWidth"].get<float>();
        if (j.contains("tileHeight"))     m.tileHeight     = j["tileHeight"].get<float>();
        if (j.contains("tilesetColumns")) m.tilesetColumns = j["tilesetColumns"].get<int>();
        if (j.contains("tilesetTexture")) m.tilesetTexture = j["tilesetTexture"].get<std::string>();
        if (j.contains("collidable"))     m.collidable     = j["collidable"].get<bool>();
        if (j.contains("tiles"))          m.tiles          = j["tiles"].get<std::vector<int>>();
    }

    // ---- AudioSource ----
    static void serializeAudio(Scene& s, EntityID e, json& j)
    {
        auto* a = s.getComponent<AudioSourceComponent>(e);
        j["clip"]     = a->clip;
        j["autoplay"] = a->autoplay;
        j["loop"]     = a->loop;
        j["volume"]   = a->volume;
    }
    static void deserializeAudio(Scene& s, EntityID e, const json& j)
    {
        auto& a = s.addComponent<AudioSourceComponent>(e);
        if (j.contains("clip"))     a.clip     = j["clip"].get<std::string>();
        if (j.contains("autoplay")) a.autoplay = j["autoplay"].get<bool>();
        if (j.contains("loop"))     a.loop     = j["loop"].get<bool>();
        if (j.contains("volume"))   a.volume   = j["volume"].get<float>();
    }

    // ---- ImGui inspectors (editor) ----
    static void inspectTransform(void* ptr)
    {
        auto* t = static_cast<TransformComponent*>(ptr);
        ImGui::DragFloat2("Position", &t->position.x, 1.0f);
        ImGui::DragFloat("Rotation", &t->rotation, 0.5f);
        ImGui::DragFloat2("Scale", &t->scale.x, 0.01f);
        ImGui::DragFloat2("Size", &t->size.x, 1.0f);
        t->updateMinMax();
    }
    static void inspectPhysics(void* ptr)
    {
        auto* p = static_cast<PhysicsComponent*>(ptr);
        ImGui::DragFloat2("Velocity", &p->velocity.x, 1.0f);
        ImGui::DragFloat("Mass", &p->mass, 0.1f);
        ImGui::Checkbox("Can Jump", &p->canJump);
    }
    static void inspectCollider(void* ptr)
    {
        auto* c = static_cast<BoxCollider*>(ptr);
        ImGui::DragFloat2("Min", &c->min.x, 1.0f);
        ImGui::DragFloat2("Max", &c->max.x, 1.0f);
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%s", c->flag.c_str());
        if (ImGui::InputText("Flag", buf, sizeof(buf))) c->flag = buf;
    }
    static void inspectCamera(void* ptr)
    {
        auto* c = static_cast<Camera*>(ptr);
        ImGui::DragFloat2("Position", &c->position.x, 1.0f);
        ImGui::DragFloat("Zoom", &c->zoom, 0.01f, 0.05f, 10.0f);
    }
    static void inspectParticle(void* ptr)
    {
        auto* p = static_cast<ParticleEmitterComponent*>(ptr);
        ImGui::Checkbox("Emitting", &p->emitting);
        ImGui::DragFloat("Emission Rate", &p->emissionRate, 1.0f, 0.0f, 1000.0f);
        ImGui::DragFloat("Particle Life", &p->particleLife, 0.05f, 0.0f, 30.0f);
        ImGui::DragInt("Max Particles", &p->maxParticles, 1.0f, 0, 100000);
        ImGui::DragFloat2("Gravity", &p->gravity.x, 1.0f);
        ImGui::DragFloat("Start Size", &p->startSize, 0.5f);
        ImGui::DragFloat("End Size", &p->endSize, 0.5f);
        ImGui::ColorEdit4("Start Color", &p->startColor.x);
        ImGui::ColorEdit4("End Color", &p->endColor.x);
        ImGui::TextDisabled("Live: %d", static_cast<int>(p->particles.size()));
    }
    static void inspectTilemap(void* ptr)
    {
        auto* m = static_cast<TilemapComponent*>(ptr);
        ImGui::Text("Grid: %d x %d", m->width, m->height);
        ImGui::DragFloat("Tile Width", &m->tileWidth, 1.0f);
        ImGui::DragFloat("Tile Height", &m->tileHeight, 1.0f);
        ImGui::DragInt("Tileset Columns", &m->tilesetColumns, 1.0f, 1, 4096);
        ImGui::Checkbox("Collidable", &m->collidable);
        ImGui::TextDisabled("Tileset: %s", m->tilesetTexture.c_str());
    }
    static void inspectAudio(void* ptr)
    {
        auto* a = static_cast<AudioSourceComponent*>(ptr);
        char buf[256];
        std::snprintf(buf, sizeof(buf), "%s", a->clip.c_str());
        if (ImGui::InputText("Clip", buf, sizeof(buf))) a->clip = buf;
        ImGui::Checkbox("Autoplay", &a->autoplay);
        ImGui::Checkbox("Loop", &a->loop);
        ImGui::DragFloat("Volume", &a->volume, 0.01f, 0.0f, 1.0f);
    }

    // ---- registry storage ----
    std::unordered_map<std::type_index, ComponentInfo>& ComponentRegistry::entries()
    {
        static std::unordered_map<std::type_index, ComponentInfo> s_entries;
        return s_entries;
    }

    std::vector<std::type_index>& ComponentRegistry::order()
    {
        static std::vector<std::type_index> s_order;
        return s_order;
    }

    const ComponentInfo* ComponentRegistry::Find(std::type_index type)
    {
        auto it = entries().find(type);
        return it == entries().end() ? nullptr : &it->second;
    }

    const ComponentInfo* ComponentRegistry::FindByName(const std::string& name)
    {
        for (auto& kv : entries())
            if (kv.second.name == name)
                return &kv.second;
        return nullptr;
    }

    std::vector<const ComponentInfo*> ComponentRegistry::All()
    {
        std::vector<const ComponentInfo*> out;
        out.reserve(order().size());
        for (std::type_index t : order())
        {
            auto it = entries().find(t);
            if (it != entries().end())
                out.push_back(&it->second);
        }
        return out;
    }

    void ComponentRegistry::RegisterBuiltins()
    {
        static bool done = false;
        if (done) return;
        done = true;

        Register<TransformComponent>("Transform",   &serializeTransform, &deserializeTransform, &inspectTransform);
        Register<PhysicsComponent>  ("Physics",     &serializePhysics,   &deserializePhysics,   &inspectPhysics);
        Register<BoxCollider>       ("BoxCollider", &serializeCollider,  &deserializeCollider,  &inspectCollider);
        Register<Camera>            ("Camera",       &serializeCamera,    &deserializeCamera,   &inspectCamera);
        Register<ParticleEmitterComponent>("ParticleEmitter", &serializeParticle, &deserializeParticle, &inspectParticle);
        Register<TilemapComponent>("Tilemap", &serializeTilemap, &deserializeTilemap, &inspectTilemap);
        Register<AudioSourceComponent>("AudioSource", &serializeAudio, &deserializeAudio, &inspectAudio);
        // Animation references textures by pointer; serialization deferred until a
        // resource-manifest is in place (kept inspectable, just not serialized).
        Register<AnimatedComponent> ("Animation");
    }

} // namespace nyl
