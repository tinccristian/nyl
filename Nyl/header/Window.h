#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "linmath.h"
#include "Log.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

namespace Nyl
{

    class Window {
    public:


        Window(int width, int height, const std::string& title);
        ~Window();

        bool Init();
        void Update();
        void Cleanup();
        bool ShouldClose() const;

        void processInput(GLFWwindow* window);
        bool getWireframeMode() { return isWireframeMode; }

        static void togglePolygonMode()
        {
            GLint polygonMode[2];
            glGetIntegerv(GL_POLYGON_MODE, polygonMode);

            if (polygonMode[0] == GL_LINE)
            {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//
            NYL_CORE_INFO("Wireframe mode set.");
            }
            else if (polygonMode[1] == GL_FILL)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//
                NYL_CORE_INFO("Line mode set.");
            }
        }

#pragma region callbacks

        static void framebuffer_size_callback(GLFWwindow*, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void error_callback(int error, const char* description);

#pragma endregion


    private:
        GLFWwindow* window;
        bool isWireframeMode=false;
        int width;
        int height;
        std::string title;
        //Shader
        Shader* shader;
        VAO* vao;
        VBO* vbo;
        EBO* ebo;
        Texture* m_texture;
        GLuint texture;
        GLuint scaleID;
    };

}
