add_executable(editor main.cpp)

target_link_libraries(editor nyl)

set_target_properties(editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT editor)
endif()

target_include_directories(editor PRIVATE
    ${PROJECT_SOURCE_DIR}/engine/src/_core/
    ${PROJECT_SOURCE_DIR}/engine/src/entity/
    ${PROJECT_SOURCE_DIR}/engine/src/component/
    ${PROJECT_SOURCE_DIR}/engine/src/system/
    ${PROJECT_SOURCE_DIR}/engine/src/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/glad/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/spdlog/include/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/imgui/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/imgui/backends/
    ${PROJECT_SOURCE_DIR}/engine/thirdparty/json/
)

target_compile_definitions(editor PRIVATE
    _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    _CONSOLE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
