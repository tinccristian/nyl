// for use by nyl applications

#include "application.h"
#include "game.h"
#include "log.h"
#include "input.h"
#include "utils.h"
#include "window.h"
#include "audio.h"
#include "audio_source.h"

// entity / ECS core
#include "scene.h"
#include "entity.h"
#include "component_registry.h"
#include "scene_serializer.h"

// render
#include "texture.h"
#include "resource_manager.h"
#include "system_renderer.h"
#include "camera.h"
#include "system_camera.h"
// physics
#include "physics.h"
#include "system_physics.h"

// colliders
#include "collider.h"
#include "system_collider.h"
#include "broadphase.h"

// particles
#include "particle.h"
#include "system_particle.h"

// tilemap
#include "tilemap.h"
#include "system_tilemap.h"

// text
#include "font_renderer.h"

// editor support
#include "framebuffer.h"

// entry point
#include "main.h"
