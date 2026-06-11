#pragma once

#include <cstdint>
#include <vector>
#include <queue>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "core.h"

namespace nyl
{
    /**
     * @brief A stable, generational handle to an entity living inside a Scene.
     *
     * `index` addresses a slot in the Scene; `generation` is bumped every time a
     * slot is reused, so a handle to a destroyed entity can be detected as stale
     * (this is what the old static-counter / free-id-queue Entity got wrong).
     * A generation of 0 means "null".
     */
    struct EntityID
    {
        std::uint32_t index = 0;
        std::uint32_t generation = 0;

        bool operator==(const EntityID& o) const { return index == o.index && generation == o.generation; }
        bool operator!=(const EntityID& o) const { return !(*this == o); }
        bool isNull() const { return generation == 0; }
    };

    inline constexpr EntityID NullEntity{ 0, 0 };

    // ---- type-erased component storage -------------------------------------

    /**
     * @brief Type-erased interface so the Scene, the component registry and the
     * editor can manipulate component pools without knowing the concrete type.
     */
    class IComponentPool
    {
    public:
        virtual ~IComponentPool() = default;
        virtual void remove(std::uint32_t index) = 0;
        virtual bool has(std::uint32_t index) const = 0;
        virtual void* getRaw(std::uint32_t index) = 0;
        virtual std::vector<std::uint32_t> indices() const = 0;
    };

    template <typename T>
    class ComponentPool : public IComponentPool
    {
    public:
        std::unordered_map<std::uint32_t, T> data;

        void remove(std::uint32_t index) override { data.erase(index); }
        bool has(std::uint32_t index) const override { return data.find(index) != data.end(); }
        void* getRaw(std::uint32_t index) override
        {
            auto it = data.find(index);
            return it == data.end() ? nullptr : static_cast<void*>(&it->second);
        }
        std::vector<std::uint32_t> indices() const override
        {
            std::vector<std::uint32_t> out;
            out.reserve(data.size());
            for (auto& kv : data) out.push_back(kv.first);
            return out;
        }
    };

    class Entity; // handle, defined in entity.h

    /**
     * @brief Owns all entities and their components.
     *
     * This is the single place that can enumerate / inspect / serialize the
     * world, which is exactly what the upcoming ImGui editor needs. Systems
     * operate over a Scene rather than holding copies of entities.
     */
    class NYL_API Scene
    {
    public:
        Scene() = default;

        // ---- entity lifecycle ----
        Entity     createEntity();      // ergonomic handle
        EntityID   createEntityID();    // raw id
        void       destroyEntity(EntityID id);
        bool       isValid(EntityID id) const;
        void       clear();

        std::vector<EntityID> allEntities() const;
        std::size_t           entityCount() const;

        // ---- components ----
        template <typename T, typename... Args>
        T& addComponent(EntityID id, Args&&... args)
        {
            ComponentPool<T>& pool = poolFor<T>();
            auto result = pool.data.try_emplace(id.index, std::forward<Args>(args)...);
            if (!result.second)
            {
                // already present: overwrite with the freshly-constructed value
                result.first->second = T(std::forward<Args>(args)...);
            }
            return result.first->second;
        }

        template <typename T>
        T* getComponent(EntityID id)
        {
            ComponentPool<T>* pool = existingPool<T>();
            if (!pool) return nullptr;
            auto it = pool->data.find(id.index);
            return it == pool->data.end() ? nullptr : &it->second;
        }

        template <typename T>
        bool hasComponent(EntityID id) const
        {
            const ComponentPool<T>* pool = existingPoolConst<T>();
            return pool && pool->data.find(id.index) != pool->data.end();
        }

        template <typename T>
        void removeComponent(EntityID id)
        {
            if (ComponentPool<T>* pool = existingPool<T>())
                pool->data.erase(id.index);
        }

        /**
         * @brief Invoke `fn(EntityID, Ts&...)` for every live entity that has
         * all of the component types `Ts`.
         *
         * Note: do not structurally modify the iterated pools from inside `fn`
         * (adding/removing the same component types, destroying entities).
         */
        template <typename... Ts, typename Fn>
        void forEach(Fn&& fn)
        {
            for (std::uint32_t index = 0; index < m_slots.size(); ++index)
            {
                if (!m_slots[index].alive) continue;
                EntityID id{ index, m_slots[index].generation };
                if ((hasComponent<Ts>(id) && ...))
                    fn(id, *getComponent<Ts>(id)...);
            }
        }

        // ---- introspection (registry / editor) ----
        IComponentPool* poolByType(std::type_index type);

    private:
        struct Slot { std::uint32_t generation = 0; bool alive = false; };

        std::vector<Slot>        m_slots;
        std::queue<std::uint32_t> m_free;
        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_pools;

        template <typename T>
        ComponentPool<T>& poolFor()
        {
            std::type_index t(typeid(T));
            auto it = m_pools.find(t);
            if (it == m_pools.end())
                it = m_pools.emplace(t, std::make_unique<ComponentPool<T>>()).first;
            return *static_cast<ComponentPool<T>*>(it->second.get());
        }

        template <typename T>
        ComponentPool<T>* existingPool()
        {
            auto it = m_pools.find(std::type_index(typeid(T)));
            return it == m_pools.end() ? nullptr : static_cast<ComponentPool<T>*>(it->second.get());
        }

        template <typename T>
        const ComponentPool<T>* existingPoolConst() const
        {
            auto it = m_pools.find(std::type_index(typeid(T)));
            return it == m_pools.end() ? nullptr : static_cast<const ComponentPool<T>*>(it->second.get());
        }
    };

} // namespace nyl
