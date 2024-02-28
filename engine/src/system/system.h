// System.h
#pragma once

#include <vector>
#include "core.h"
#include "entity.h"

/**
 * @class System
 * @brief This class represents a system in an Entity-Component-System (ECS) architecture.
 * 
 * The System class serves as a base class for all types of systems.
 * Each system operates on Entities with certain Components.
 * 
 * This class is part of the NYL engine and is exported as a DLL.
 */
class NYL_API System {
public:
    /**
     * @brief Construct a new System object.
     * 
     * This constructor is empty, as the System class serves only as a base class.
     */
    System() = default;

    /**
     * @brief Add an Entity to this system.
     * 
     * @param entity A reference to the Entity to add.
     */
    void addEntity(Entity& entity) {
        entities.push_back(entity);
    }

    /**
     * @brief Update all Entities in this system.
     * 
     * This method is virtual and is intended to be overridden by subclasses.
     * The default implementation does nothing.
     */
    virtual void update() {}

    /**
     * @brief Destroy the System object.
     * 
     * This destructor is virtual, as the System class is intended to be a base class.
     */
    virtual ~System() = default;

protected:
    std::vector<Entity> entities; ///< The Entities this system operates on.
};