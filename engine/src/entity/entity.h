#pragma once

#include <utility>

#include "core.h"
#include "scene.h"

namespace nyl
{
    /**
     * @class Entity
     * @brief A lightweight, copyable handle into a Scene.
     *
     * An Entity no longer owns its components; it is just an `{ id, scene }`
     * pair that forwards to the Scene. This removes the old by-value copy /
     * id-recycling bug: copying an Entity is cheap and safe, and systems can
     * hold handles (or ids) without duplicating component data.
     */
    class NYL_API Entity
    {
    public:
        Entity() = default;
        Entity(EntityID id, Scene* scene) : m_id(id), m_scene(scene) {}

        EntityID id() const { return m_id; }
        Scene*   scene() const { return m_scene; }
        bool     valid() const { return m_scene && m_scene->isValid(m_id); }

        explicit operator bool() const { return valid(); }
        bool operator==(const Entity& o) const { return m_id == o.m_id && m_scene == o.m_scene; }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            return m_scene->addComponent<T>(m_id, std::forward<Args>(args)...);
        }

        template <typename T>
        T* getComponent() const
        {
            return m_scene->getComponent<T>(m_id);
        }

        template <typename T>
        bool hasComponent() const
        {
            return m_scene->hasComponent<T>(m_id);
        }

        template <typename T>
        void removeComponent()
        {
            m_scene->removeComponent<T>(m_id);
        }

        void destroy()
        {
            if (m_scene) m_scene->destroyEntity(m_id);
        }

    private:
        EntityID m_id{};
        Scene*   m_scene = nullptr;
    };

} // namespace nyl
