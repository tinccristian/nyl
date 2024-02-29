#pragma once
#include "system.h"
#include "camera.h"
#include "transform.h"
//#include <GL/glew.h>

namespace Nyl {
    class NYL_API CameraSystem : public System {
    public:
        CameraSystem(Camera &camera);
        ~CameraSystem();

        void update(Entity& entity);
        void initCameraData();
        void CheckGLError();

    private:
        Camera &camera;
    };
}