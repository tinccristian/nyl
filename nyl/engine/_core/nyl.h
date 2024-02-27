//For use by Nyl applications

#include "core_application.h"
#include "core_game.h"
#include "core_log.h"
#include "core_input.h"
//#include "GameObject.h"
#include "component_texture.h"
#include "resource_manager.h"
#include "component_render.h"
#include "system_renderer.h"
#include "component_camera.h"
#include "system_camera.h"
//physics
#include "component_physics.h"
#include "system_physics.h"

//colliders
#include "component_collider.h"
#include "system_collider.h"

//entry point
#include "core_main.h"

// Vertex Data[] -> Vertex Shader -> Shape Assembly -> Geometry Shader -> Rasterization (geometry to pixels) -> fragment shader (coloring of the pixels) -> blending, etc (fx)