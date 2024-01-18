# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Set the project name
project(Antares)

# Gather all source files from Nyl/source
file(GLOB NYL_SOURCES "Nyl/source/*.cpp")

# Gather all header files from Nyl/headers
file(GLOB NYL_HEADERS "Nyl/headers/*.h")



add_library(glad include/glad/glad.h include/glad/glad.c)
target_include_directories(glad PUBLIC include/)
# Add the source files for the DLL
add_library(Nyl SHARED ${NYL_SOURCES} ${NYL_HEADERS})
target_include_directories(Nyl PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/glad/
    ${PROJECT_SOURCE_DIR}/include/GLFW/
    ${PROJECT_SOURCE_DIR}/include/glm/
    ${PROJECT_SOURCE_DIR}/include/stb/
)
target_include_directories(Nyl PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Nyl
    ${PROJECT_SOURCE_DIR}/Nyl/header
    ${PROJECT_SOURCE_DIR}/Nyl/thirdparty/spdlog/include)

include_directories(${CMAKE_SOURCE_DIR}/Nyl/thirdparty/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/Nyl)
# Add the glfw3 library directory
# link the GLFW library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(include/glfw-3.3.9)

# Link against the GLFW and GLAD libraries
target_link_libraries(Nyl glfw glad)

# Preprocessor definitions
target_compile_definitions(Nyl PRIVATE 
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    NYL_PLATFORM_WINDOWS
    NYL_BUILD_DLL
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
# Set the output directory for the Nyl executable
set_target_properties(Nyl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/output)
############################### ANTARES ########################################
# Add the source files
add_executable(Antares Antares/source/Antares.cpp)

# Link the DLL with the executable
target_link_libraries(Antares Nyl)


# Set the output directory for the Antares executable
set_target_properties(Antares PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/output)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the platform
if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(PLATFORM x64)
else()
    set(PLATFORM x86)
endif()

# set include directories
target_include_directories(Antares PRIVATE 
    ${PROJECT_SOURCE_DIR}/Antares
    ${PROJECT_SOURCE_DIR}/Nyl
    ${PROJECT_SOURCE_DIR}/Antares/header
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