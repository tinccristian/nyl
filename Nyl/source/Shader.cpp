#include "Shader.h"
#include "Log.h"
using namespace Nyl;


    

Shader::Shader() { ID = 0; }
Shader& Shader::use()
{
    glUseProgram(this->ID);
    return *this;
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int sVertex, sFragment, gShader;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    check_compile_errors(sVertex, "VERTEX");
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    check_compile_errors(sFragment, "FRAGMENT");
    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        check_compile_errors(gShader, "GEOMETRY");
    }
    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(this->ID, gShader);
    glLinkProgram(this->ID);
    check_compile_errors(this->ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);

}


void Shader::set_int(const char* name, int value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}


void Shader::set_float(const char * name, float value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::set_vec3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::set_vec3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::set_mat4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}
void Shader::set_vec2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::set_vec2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::check_compile_errors(unsigned int shader, std::string type)
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
