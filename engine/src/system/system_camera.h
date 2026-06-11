#pragma once
#include "system.h"
#include "entity.h"
#include "camera.h"
#include "transform.h"

namespace nyl {
	class NYL_API CameraSystem : public System {
	public:
		CameraSystem() = default;
		~CameraSystem() override = default;

		/// Make the entity's Camera follow the entity's Transform.
		void update(Entity& entity);

	private:
		void CheckGLError();
	};
}
