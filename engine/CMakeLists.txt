set(NYL_SOURCES 
    #core
    "src/_core/resource_manager.cpp"
    "src/_core/core_game.cpp"
    "src/_core/core_input.cpp"
    "src/_core/core_log.cpp"
    "src/_core/core_application.cpp"
    #entity
    "src/entity/entity.cpp"
    #component
    "src/component/component_shader.cpp"
    "src/component/component_texture.cpp"
    #system
    "src/system/system_collider.cpp"
    "src/system/system_physics.cpp"
    "src/system/system_renderer.cpp"
    "src/system/system_shader.cpp"
    "src/system/system_camera.cpp"
    #thirdparty
    "src/stb.cpp"
)

set(NYL_HEADERS 
    #core
    "src/_core/core.h"
    "src/_core/core_application.h"
    "src/_core/core_game.h"
    "src/_core/core_input.h"
    "src/_core/core_log.h"
    "src/_core/core_main.h"
    "src/_core/resource_manager.h"
    "src/_core/nyl.h"
    #entity
    "src/entity/entity.h"
    "src/entity/entity_manager.h"
    #component
    "src/component/component.h"
    "src/component/component_collider.h"
    "src/component/component_physics.h"
    "src/component/component_render.h"
    "src/component/component_shader.h"
    "src/component/component_texture.h"
    "src/component/component_transform.h"
    "src/component/component_camera.h"
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
    ${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include/
)
include_directories(${PROJECT_SOURCE_DIR}/engine/thirdparty/spdlog/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_library(GLFW_LIBRARY glfw3 PATHS D:/gitHub/nyl/engine/thirdparty/glfw-3.3.9/build/Debug/src)
else()
    find_library(GLFW_LIBRARY glfw3 PATHS D:/gitHub/nyl/engine/thirdparty/glfw-3.3.9/build/Release/src)
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