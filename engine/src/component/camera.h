#pragma once

#include "core.h"
#include "component.h"
#include <glad/glad.h>
#include "glm/glm.hpp"

namespace nyl
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
        bool locked;
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
        Camera(float x, float y, float width, float height, float zoom = 1.0f, bool locked = false)
            : x(x), y(y), width(width), height(height), zoom(zoom),locked(locked) 
            {
                initCameraData();
            }
        void initCameraData()
        {
            position = glm::vec3(0.0f, 0.0f, 0.0f);
            // Initialize camera data here
        }
        void update(glm::vec2 position)
        {
            if (locked)
            {
                this->position.x = position.x;
                this->position.y = position.y;
            }
            this->CheckGLError();
        }
        void CheckGLError()
        {
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR)
            {
                NYL_CORE_ERROR("OpenGL error: " + std::to_string(err));
            }
        }
    };
}