# specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# set the project name
project(Antares)

# set the NINJA_STATUS environment variable
set(ENV{NINJA_STATUS} "[\\033[1;34m%u/%t\\033[0m (\\033[1;32m%f\\033[0m)] ")
############################### NYL ########################################

# gather all source files from nyl
set(NYL_SOURCES 
    #core
    "nyl/engine/_core/core_application.cpp"
    "nyl/engine/_core/resource_manager.cpp"
    "nyl/engine/_core/core_game.cpp"
    "nyl/engine/_core/core_input.cpp"
    "nyl/engine/_core/core_log.cpp"
    #entity
    "nyl/engine/entity/entity.cpp"
    #component
    "nyl/engine/component/component_shader.cpp"
    "nyl/engine/component/component_texture.cpp"
    #system
    "nyl/engine/system/system_collider.cpp"
    "nyl/engine/system/system_physics.cpp"
    "nyl/engine/system/system_renderer.cpp"
    "nyl/engine/system/system_shader.cpp"
    #thirdparty
    "nyl/engine/stb.cpp"
)
# gather all header files from nyl
set(NYL_HEADERS 
    #core
    "nyl/engine/_core/core.h"
    "nyl/engine/_core/core_application.h"
    "nyl/engine/_core/core_game.h"
    "nyl/engine/_core/core_input.h"
    "nyl/engine/_core/core_log.h"
    "nyl/engine/_core/core_main.h"
    "nyl/engine/_core/resource_manager.h"
    "nyl/engine/_core/nyl.h"
    #entity
    "nyl/engine/entity/entity.h"
    "nyl/engine/entity/entity_manager.h"
    #component
    "nyl/engine/component/component.h"
    "nyl/engine/component/component_collider.h"
    "nyl/engine/component/component_physics.h"
    "nyl/engine/component/component_render.h"
    "nyl/engine/component/component_shader.h"
    "nyl/engine/component/component_texture.h"
    "nyl/engine/component/component_transform.h"
    #system
    "nyl/engine/system/system.h"
    "nyl/engine/system/system_collider.h"
    "nyl/engine/system/system_physics.h"
    "nyl/engine/system/system_renderer.h"
    #thirdparty
    "nyl/engine/linmath.h"
)

add_library(glad include/glad/glad.h include/glad/glad.c)
target_include_directories(glad PUBLIC include/)
# add the source files for the DLL
add_library(nyl SHARED ${NYL_SOURCES} ${NYL_HEADERS})
set_target_properties(nyl PROPERTIES OUTPUT_NAME "__nylengine" PREFIX "")
target_include_directories(nyl PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/glad/
    ${PROJECT_SOURCE_DIR}/include/GLFW/
    ${PROJECT_SOURCE_DIR}/include/glm/
    ${PROJECT_SOURCE_DIR}/include/stb/
    ${PROJECT_SOURCE_DIR}/nyl/engine/
    ${PROJECT_SOURCE_DIR}/nyl/engine/component
)
target_include_directories(nyl PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/nyl/engine/_core/
    ${PROJECT_SOURCE_DIR}/nyl/engine/entity/
    ${PROJECT_SOURCE_DIR}/nyl/engine/component/
    ${PROJECT_SOURCE_DIR}/nyl/engine/system/
    ${PROJECT_SOURCE_DIR}/nyl/thirdparty/spdlog/include)

include_directories(${CMAKE_SOURCE_DIR}/nyl/thirdparty/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/nyl)
# add the glfw3 library directory
# link the GLFW library
# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# add_subdirectory(include/glfw-3.3.9)

# # link against the GLFW and GLAD libraries
# target_link_libraries(nyl glfw glad)

# Find the pre-built GLFW library depending on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_library(GLFW_LIBRARY glfw3 PATHS D:/gitHub/nyl/include/glfw-3.3.9/build/Debug/src)
else()
    find_library(GLFW_LIBRARY glfw3 PATHS D:/gitHub/nyl/include/glfw-3.3.9/build/Release/src)
endif()

# Link against the GLFW and GLAD libraries
target_link_libraries(nyl ${GLFW_LIBRARY} glad)
# preprocessor definitions
target_compile_definitions(nyl PRIVATE 
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    NYL_PLATFORM_WINDOWS
    NYL_BUILD_DLL
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
# set the output directory for the nyl executable
set_target_properties(nyl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/output)
############################### ANTARES ########################################
# add the source files
add_executable(Antares Antares/source/Antares.cpp)

# link the DLL with the executable
target_link_libraries(Antares nyl)


# Set the output directory for the Antares executable
set_target_properties(Antares PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/output)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the platform
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(PLATFORM x64)
else()
    set(PLATFORM x86)
endif()

# set include directories
target_include_directories(Antares PRIVATE 
    ${PROJECT_SOURCE_DIR}/Antares
    ${PROJECT_SOURCE_DIR}/Antares/header
    ${PROJECT_SOURCE_DIR}/nyl/engine/_core/
    ${PROJECT_SOURCE_DIR}/nyl/engine/entity/
    ${PROJECT_SOURCE_DIR}/nyl/engine/component/
    ${PROJECT_SOURCE_DIR}/nyl/engine/system/
)

# set preprocessor definitions
target_compile_definitions(Antares PRIVATE 
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    NYL_PLATFORM_WINDOWS
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
# set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# set the preprocessor definitions
target_compile_definitions(Antares PRIVATE NYL_PLATFORM_WINDOWS)