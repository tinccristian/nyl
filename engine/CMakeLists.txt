set(NYL_SOURCES 
    #core
    "src/_core/resource_manager.cpp"
    "src/_core/game.cpp"
    "src/_core/input.cpp"
    "src/_core/log.cpp"
    "src/_core/application.cpp"
    "src/_core/utils.cpp"
    "src/_core/window.cpp"
    #entity
    "src/entity/entity.cpp"
    #component
    "src/component/shader.cpp"
    "src/component/texture.cpp"
    #system
    "src/system/system_collider.cpp"
    "src/system/system_physics.cpp"
    "src/system/system_renderer.cpp"
    "src/system/system_shader.cpp"
    "src/system/system_camera.cpp"
    #thirdparty
    "thirdparty/stb/stb.cpp"
)

set(NYL_HEADERS 
    #core
    "src/_core/core.h"
    "src/_core/application.h"
    "src/_core/game.h"
    "src/_core/input.h"
    "src/_core/log.h"
    "src/_core/main.h"
    "src/_core/resource_manager.h"
    "src/_core/utils.h"
    "src/_core/window.h"
    "src/nyl.h"
    #entity
    "src/entity/entity.h"
    "src/entity/entity_manager.h"
    #component
    "src/component/component.h"
    "src/component/collider.h"
    "src/component/physics.h"
    "src/component/render.h"
    "src/component/shader.h"
    "src/component/texture.h"
    "src/component/transform.h"
    "src/component/camera.h"
    #system
    "src/system/system.h"
    "src/system/system_collider.h"
    "src/system/system_physics.h"
    "src/system/system_renderer.h"
    "src/system/system_camera.h"
    #thirdparty
)

add_library(glad thirdparty/glad/glad.h thirdparty/glad/glad.c)
target_include_directories(glad PUBLIC thirdparty/)

add_library(nyl SHARED ${NYL_SOURCES} ${NYL_HEADERS})
set_target_properties(nyl PROPERTIES OUTPUT_NAME "__nylengine" PREFIX "")

target_include_directories(nyl PRIVATE 
    thirdparty/glad/
    thirdparty/GLFW/
    thirdparty/glm/
    thirdparty/stb/
    src/
    src/component
    src/_core/
    src/entity/
    src/component/
    src/system/
)
include_directories(${PROJECT_SOURCE_DIR}/engine/thirdparty/spdlog/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_library(GLFW_LIBRARY glfw3 PATHS thirdparty/glfw-3.3.9/build/Debug/src)
else()
    find_library(GLFW_LIBRARY glfw3 PATHS thirdparty/glfw-3.3.9/build/Release/src)
endif()

target_link_libraries(nyl ${GLFW_LIBRARY} glad)

target_compile_definitions(nyl PRIVATE 
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    NYL_PLATFORM_WINDOWS
    NYL_BUILD_DLL
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

set_target_properties(nyl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/output)