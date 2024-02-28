add_executable(Antares source/Antares.cpp)

target_link_libraries(Antares nyl)

set_target_properties(Antares PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/output)

target_include_directories(Antares PRIVATE 
    ${PROJECT_SOURCE_DIR}/examples/Antares
    ${PROJECT_SOURCE_DIR}/examples/Antares/header/
    ${PROJECT_SOURCE_DIR}/engine/src/_core/
    ${PROJECT_SOURCE_DIR}/engine/src/entity/
    ${PROJECT_SOURCE_DIR}/engine/src/component/
    ${PROJECT_SOURCE_DIR}/engine/src/system/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/spdlog/include/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/
)

target_compile_definitions(Antares PRIVATE 
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    NYL_PLATFORM_WINDOWS
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)