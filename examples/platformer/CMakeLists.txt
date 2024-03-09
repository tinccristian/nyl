add_executable(platformer source/platformer.cpp)

target_link_libraries(platformer nyl)

set_target_properties(platformer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/output)

target_include_directories(platformer PRIVATE 
    ${PROJECT_SOURCE_DIR}/examples/platformer/header/
    ${PROJECT_SOURCE_DIR}/engine/src/_core/
    ${PROJECT_SOURCE_DIR}/engine/src/entity/
    ${PROJECT_SOURCE_DIR}/engine/src/component/
    ${PROJECT_SOURCE_DIR}/engine/src/system/
    ${PROJECT_SOURCE_DIR}/engine/src/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/spdlog/include/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/imgui/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/imgui/backends/
)

target_compile_definitions(platformer PRIVATE 
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    NYL_PLATFORM_WINDOWS
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)