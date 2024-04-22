#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "entity.h"

/**
 * @class EntityManager
 * @brief This class manages entities in an Entity-Component-System (ECS) architecture.
 * 
 * The EntityManager class is responsible for creating, updating, and deleting entities.
 * 
 * This class is part of the NYL engine and is exported as a DLL.
 */
class NYL_API EntityManager 
{
public:
    /**
     * @brief Create a new entity with a unique ID.
     * 
     * @return Entity* A pointer to the newly created entity.
     */
    Entity* createEntity() 
    {
        int id = entities.size();
        Entity* entity = new Entity();
        entities.push_back(std::unique_ptr<Entity>(entity));
        return entity;
    }

    /**
     * @brief Update all entities.
     * 
     * This method loops over all entities and updates their components.
     */
    void update(float deltaTime) 
    {
        for (auto& entity : entities) 
        {
            // Update the entity's components here.
            // This will depend on your specific component implementation.
        }
    }

    /**
     * @brief Delete an entity.
     * 
     * @param entity The entity to delete.
     */

    void deleteEntity(Entity* entity) 
    {
        // Find and remove the entity from the entities vector.
        // This will automatically delete the entity and its components because of the unique_ptr.
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [entity](const std::unique_ptr<Entity>& e) { return e.get() == entity; }),
            entities.end());
    }

private:
    std::vector<std::unique_ptr<Entity>> entities; ///< The entities managed by this manager.
};