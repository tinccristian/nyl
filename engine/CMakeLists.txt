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
    "src/entity/player_entity.cpp"
    #component
    "src/component/shader.cpp"
    "src/component/texture.cpp"
    #event
    "src/event_system/Event.cpp"
    #system
    "src/system/system_collider.cpp"
    "src/system/system_physics.cpp"
    "src/system/system_renderer.cpp"
    "src/system/system_shader.cpp"
    "src/system/system_camera.cpp"
    #thirdparty
    "thirdparty/stb/stb.cpp"
    "thirdparty/imgui/imgui.cpp"
    "thirdparty/imgui/imgui_demo.cpp" # If you want to include the demo windows
    "thirdparty/imgui/imgui_draw.cpp"
    "thirdparty/imgui/imgui_widgets.cpp"
    "thirdparty/imgui/imgui_tables.cpp"
    "thirdparty/imgui/backends/imgui_impl_glfw.cpp"
    "thirdparty/imgui/backends/imgui_impl_opengl3.cpp"
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
    "src/entity/player_entity.h"
    #event system
    "src/event_system/ApplicationEvent.h"
    "src/event_system/Event.h"
    "src/event_system/KeyEvent.h"
    "src/event_system/MouseEvent.h"
    #component
    "src/component/component.h"
    "src/component/collider.h"
    "src/component/physics.h"
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
    "thirdparty/imgui/imgui.h"
    "thirdparty/imgui/imgui_internal.h"
    "thirdparty/imgui/imstb_rectpack.h"
    "thirdparty/imgui/imstb_textedit.h" 
    "thirdparty/imgui/imstb_truetype.h" 
    "thirdparty/imgui/backends/imgui_impl_glfw.h"
    "thirdparty/imgui/backends/imgui_impl_opengl3.h"
)

add_library(glad SHARED thirdparty/glad/glad.h thirdparty/glad/glad.c)
set_property(TARGET glad PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(glad PUBLIC thirdparty/)

add_library(nyl SHARED ${NYL_SOURCES} ${NYL_HEADERS})
set_property(TARGET nyl PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(nyl PROPERTIES OUTPUT_NAME "__nylengine" PREFIX "")

target_include_directories(nyl PRIVATE 
    thirdparty/glad/
    thirdparty/glm/
    thirdparty/stb/
    thirdparty/imgui/
    thirdparty/imgui/backends/
    src/
    src/component
    src/_core/
    src/entity/
    src/component/
    src/system/
    src/event_system/
)
include_directories(${PROJECT_SOURCE_DIR}/engine/thirdparty/spdlog/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)

# link the GLFW library
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(nyl ${GLFW_LIBRARIES} glad)

target_compile_definitions(nyl PRIVATE 
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    NYL_BUILD_DLL
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

set_target_properties(nyl PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/$<CONFIG>)