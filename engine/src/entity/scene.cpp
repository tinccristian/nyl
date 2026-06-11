#include "scene.h"
#include "entity.h"

namespace nyl
{
    Entity Scene::createEntity()
    {
        return Entity(createEntityID(), this);
    }

    EntityID Scene::createEntityID()
    {
        std::uint32_t index;
        if (!m_free.empty())
        {
            index = m_free.front();
            m_free.pop();
        }
        else
        {
            index = static_cast<std::uint32_t>(m_slots.size());
            m_slots.push_back(Slot{ 0, false });
        }

        Slot& slot = m_slots[index];
        slot.alive = true;
        if (slot.generation == 0) slot.generation = 1; // 0 is reserved for "null"
        return EntityID{ index, slot.generation };
    }

    void Scene::destroyEntity(EntityID id)
    {
        if (!isValid(id)) return;

        const std::uint32_t i = id.index;
        for (auto& kv : m_pools)
            kv.second->remove(i);

        m_slots[i].alive = false;
        ++m_slots[i].generation; // invalidate any outstanding handles
        m_free.push(i);
    }

    bool Scene::isValid(EntityID id) const
    {
        return id.generation != 0
            && id.index < m_slots.size()
            && m_slots[id.index].alive
            && m_slots[id.index].generation == id.generation;
    }

    void Scene::clear()
    {
        m_pools.clear();
        m_slots.clear();
        std::queue<std::uint32_t> empty;
        m_free.swap(empty);
    }

    std::vector<EntityID> Scene::allEntities() const
    {
        std::vector<EntityID> out;
        out.reserve(m_slots.size());
        for (std::uint32_t i = 0; i < m_slots.size(); ++i)
            if (m_slots[i].alive)
                out.push_back(EntityID{ i, m_slots[i].generation });
        return out;
    }

    std::size_t Scene::entityCount() const
    {
        std::size_t n = 0;
        for (const Slot& s : m_slots)
            if (s.alive) ++n;
        return n;
    }

    IComponentPool* Scene::poolByType(std::type_index type)
    {
        auto it = m_pools.find(type);
        return it == m_pools.end() ? nullptr : it->second.get();
    }

} // namespace nyl
