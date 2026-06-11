#include "system_camera.h"
#include "log.h"
#include "camera.h"
#include "transform.h"
#include <glad/glad.h>

namespace nyl
{
    void CameraSystem::update(Entity& entity)
    {
        auto camera = entity.getComponent<Camera>();
        auto transform = entity.getComponent<TransformComponent>();
        if (!camera || !transform) {
            NYL_CORE_ERROR("Error: Entity does not have a Camera or TransformComponent");
            return;
        }

        camera->position.x = transform->position.x;
        camera->position.y = transform->position.y;

        this->CheckGLError();
    }

    void CameraSystem::CheckGLError()
    {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            NYL_CORE_ERROR("OpenGL error: " + std::to_string(err));
        }
    }
}
