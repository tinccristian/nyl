cmake_minimum_required(VERSION 3.20)
project(platformer)

# Set global C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ENV{NINJA_STATUS} "[\\033[1;34m%u/%t\\033[0m (\\033[1;32m%f\\033[0m)] ")

add_subdirectory(engine)
add_subdirectory(examples/platformer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)