#pragma once

#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


namespace Nyl
{
    class Shader
    {
    public:

        // state
        unsigned int ID;
        // empty constructor
        Shader();
        //// constructor generates the shader on the fly
        //Shader(const char* vertexPath, const char* fragmentPath);
        // activate the shader
        Shader& use();

        // compile shader from source
        void    Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); 

        // utility uniform functions
        //void setBool(const std::string& name, bool value);
        void    setInt(const char* name, int value, bool useShader = false);
        void    setFloat(const char* name, float value, bool useShader = false);
        void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
        void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
        void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

    private:
        // utility function for checking shader compilation/linking errors.
        static void checkCompileErrors(unsigned int shader, std::string type);
    };
}