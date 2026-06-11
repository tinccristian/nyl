#pragma once

#include <string>
#include <vector>
#include <typeindex>
#include <unordered_map>

#include <nlohmann/json_fwd.hpp>

#include "core.h"
#include "scene.h"

namespace nyl
{
    /**
     * @brief Runtime description of a component type.
     *
     * Holds type-erased operations so generic code (scene serialization in
     * Phase 6, the ImGui editor inspector in Phase 8) can add / query / remove
     * / access components by name without compile-time knowledge of the type.
     */
    struct ComponentInfo
    {
        std::string     name;
        std::type_index type;

        void  (*addDefault)(Scene&, EntityID);
        bool  (*has)(Scene&, EntityID);
        void  (*remove)(Scene&, EntityID);
        void* (*getRaw)(Scene&, EntityID);

        // serialization (null if the component is not serializable). Implemented
        // in component_registry.cpp where the full json header is available.
        void  (*serialize)(Scene&, EntityID, nlohmann::json&);
        void  (*deserialize)(Scene&, EntityID, const nlohmann::json&);

        // ImGui inspector drawing the component's fields (null if none). Takes
        // the raw component pointer (cast from getRaw). Used by the editor.
        void  (*inspect)(void*);
    };

    /**
     * @brief Global registry mapping component type-names <-> type operations.
     *
     * Components are registered once at engine init via RegisterBuiltins().
     * Game code can register its own component types the same way.
     */
    class NYL_API ComponentRegistry
    {
    public:
        template <typename T>
        static void Register(const std::string& name,
            void (*serialize)(Scene&, EntityID, nlohmann::json&) = nullptr,
            void (*deserialize)(Scene&, EntityID, const nlohmann::json&) = nullptr,
            void (*inspect)(void*) = nullptr)
        {
            ComponentInfo info{
                name,
                std::type_index(typeid(T)),
                [](Scene& s, EntityID e) { s.addComponent<T>(e); },
                [](Scene& s, EntityID e) { return s.hasComponent<T>(e); },
                [](Scene& s, EntityID e) { s.removeComponent<T>(e); },
                [](Scene& s, EntityID e) -> void* { return static_cast<void*>(s.getComponent<T>(e)); },
                serialize,
                deserialize,
                inspect
            };
            // operator[] would need a default-constructible ComponentInfo
            // (type_index has no default ctor), so insert_or_assign instead.
            entries().insert_or_assign(info.type, info);
            // keep a stable, insertion-ordered list for menus
            order().push_back(info.type);
        }

        static const ComponentInfo* Find(std::type_index type);
        static const ComponentInfo* FindByName(const std::string& name);
        static std::vector<const ComponentInfo*> All();

        /// Copy every serializable component from `from` onto `to` (used by the
        /// editor's "Duplicate"). Components without serialization are skipped.
        static void CloneEntity(Scene& scene, EntityID from, EntityID to);

        /// Register all engine-provided component types. Call once at startup.
        static void RegisterBuiltins();

    private:
        static std::unordered_map<std::type_index, ComponentInfo>& entries();
        static std::vector<std::type_index>& order();
    };

} // namespace nyl
