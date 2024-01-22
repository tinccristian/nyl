#pragma once
#include "Core.h"
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


namespace Nyl
{
    class NYL_API Shader
    {
    public:

        // state
        unsigned int ID;
        // empty constructor
        Shader();
        // activate the shader
        Shader& use();

        // compile shader from source
        void    compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

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
}