#pragma once
#include "system.h"
#include "camera.h"
#include "transform.h"
//#include <GL/glew.h>

namespace nyl {
	class NYL_API CameraSystem : public System {
	public:
		CameraSystem(std::shared_ptr<Camera> camera); // Change parameter type to shared_ptr
		~CameraSystem() = default;

		void update(Entity& entity);
		void initCameraData();
		void CheckGLError();

	private:
		std::shared_ptr<Camera> camera; // Change member type to shared_ptr
	};
}
