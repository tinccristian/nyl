#include "system_camera.h"
#include "log.h"
#include "transform.h"
#include "camera.h"
#include <glad.h>

namespace nyl
{
    CameraSystem::CameraSystem(Camera &camera)
        : camera(camera)
    {
        this->initCameraData();
    }

    CameraSystem::~CameraSystem()
    {
        // Add any necessary cleanup here
    }

    void CameraSystem::update(Entity& entity) 
    {
        auto camera = entity.getComponent<Camera>();
        auto transform = entity.getComponent<TransformComponent>();
        if (!camera || !transform) {
            NYL_CORE_ERROR("Error: Entity does not have a CameraComponent or TransformComponent");
            return;
        }
        
        camera->position.x = transform->position.x;
        camera->position.y = transform->position.y;
        
        // // Define the size of the deadzone
        // float deadzone = 100.0f;

        // // Only update the camera's position if the player has moved out of the deadzone
        // if (abs(camera->position.x - transform->position.x) > deadzone) {
        //     camera->position.x = transform->position.x;
        // }
        // if (abs(camera->position.y - transform->position.y) > deadzone) {
        //     camera->position.y = transform->position.y;
        // }

        // Check for OpenGL errors after updating the camera
        this->CheckGLError();
    }

    void CameraSystem::initCameraData()
    {
        camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
        // Initialize camera data here
    }

    void CameraSystem::CheckGLError()
    {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            NYL_CORE_ERROR("OpenGL error: " + std::to_string(err));
        }
    }
}