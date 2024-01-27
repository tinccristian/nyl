// Component.h
#pragma once

#include "core.h"
#include "entity.h"

/**
 * @class Component
 * @brief This class represents a component in an Entity-Component-System (ECS) architecture.
 * 
 * This class is part of the NYL engine and is exported as a DLL.
 */
class NYL_API Component {
public:
    /**
     * @brief Construct a new Component object
     * 
     * @param id The unique ID for this component.
     * @param entity A reference to the Entity this component belongs to.
     */
    Component() = default;

    // /**
    //  * @brief Get the ID of this component.
    //  * 
    //  * @return int The unique ID of this component.
    //  */
    // int getId() const { return id; }

    // /**
    //  * @brief Get the Entity this component belongs to.
    //  * 
    //  * @return Entity& A reference to the Entity this component belongs to.
    //  */
    // Entity& getEntity() const { return entity; }

    /**
     * @brief Destroy the Component object.
     * 
     * This destructor is virtual, as the Component class is intended to be a base class.
     */
    virtual ~Component() = default;

// private:
//     int id; ///< The unique ID for this component.
//     Entity& entity; ///< A reference to the Entity this component belongs to.
};