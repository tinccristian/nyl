#pragma once
#include "core.h"
#include "component.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <string>

class NYL_API ShaderComponent : public Component {
public:
    ShaderComponent() : Component() {}

    unsigned int ID;
    glm::mat4 matrix;
    std::string vertexSource;
    std::string fragmentSource;
    // activate the shader
    ShaderComponent& use();
    void compile();
    // utility functions
    void    set_float(const char* name, float value, bool useShader = false);
    void    set_int(const char* name, int value, bool useShader = false);
    void    set_vec2f(const char* name, float x, float y, bool useShader = false);
    void    set_vec2f(const char* name, const glm::vec2& value, bool useShader = false);
    void    set_vec3f(const char* name, float x, float y, float z, bool useShader = false);
    void    set_vec3f(const char* name, const glm::vec3& value, bool useShader = false);
    void    set_vec4f(const char* name, float x, float y, float z, float w, bool useShader = false);
    void    set_vec4f(const char* name, const glm::vec4& value, bool useShader = false);
    void    set_mat4(const char* name, const glm::mat4& matrix, bool useShader = false);
private:
    // utility function for checking shader compilation/linking errors.
    static void check_compile_errors(unsigned int shader, std::string type);
};