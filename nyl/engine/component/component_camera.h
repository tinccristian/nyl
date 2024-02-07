#pragma once

#include "core.h"
#include "component.h"
#include "glm/glm.hpp"

namespace Nyl
{
    /**
     * @brief The Camera component is used to define the camera position and viewport.
     * 
     */
    class NYL_API Camera : public Component {
    public:
        float x, y;
        float width, height;
        float zoom; 
        glm::vec2 position;
        /**
         * @brief Constructs a new Camera object.
         * 
         * @param x The x-coordinate of the camera position.
         * @param y The y-coordinate of the camera position.
         * @param width The width of the camera viewport.
         * @param height The height of the camera viewport.
         * @param zoom The zoom level of the camera (default is 1.0f).
         */
        Camera(float x, float y, float width, float height, float zoom = 1.0f)
            : x(x), y(y), width(width), height(height), zoom(zoom) {}
    };
}