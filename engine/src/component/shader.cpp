// ShaderComponent.cpp
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "log.h"


ShaderComponent& ShaderComponent::use()
{
    glUseProgram(this->ID);
    return *this;
}

void ShaderComponent::compile() {
    // Compile the shader
    unsigned int sVertex, sFragment;
    const char* vertexSource = this->vertexSource.c_str();
    const char* fragmentSource = this->fragmentSource.c_str();

    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    check_compile_errors(sVertex, "VERTEX"); // You need to implement this function

    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    check_compile_errors(sFragment, "FRAGMENT"); // You need to implement this function

    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    glLinkProgram(this->ID);
    check_compile_errors(this->ID, "PROGRAM"); // You need to implement this function

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}
void ShaderComponent::set_int(const char* name, int value, bool useShader) {
    if (useShader)
        glUseProgram(ID);
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void ShaderComponent::set_float(const char* name, float value, bool useShader) {
    if (useShader)
        glUseProgram(ID);
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void ShaderComponent::set_vec3f(const char* name, float x, float y, float z, bool useShader) {
    if (useShader)
        glUseProgram(ID);
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void ShaderComponent::set_vec3f(const char* name, const glm::vec3& value, bool useShader) {
    if (useShader)
        glUseProgram(ID);
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}

void ShaderComponent::set_mat4(const char* name, const glm::mat4& matrix, bool useShader) {
    if (useShader)
        glUseProgram(ID);
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
}

void ShaderComponent::set_vec2f(const char* name, float x, float y, bool useShader) {
    if (useShader)
        glUseProgram(ID);
    glUniform2f(glGetUniformLocation(ID, name), x, y);
}

void ShaderComponent::set_vec2f(const char* name, const glm::vec2& value, bool useShader) {
    if (useShader)
        glUseProgram(ID);
    glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}
void ShaderComponent::check_compile_errors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
        if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            NYL_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}\n^^ <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> --\n", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            NYL_CORE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}\n^^ <><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><> --\n", type, infoLog);
        }
    }
}