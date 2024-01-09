#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "linmath.h"
#include "Log.h"


namespace Nyl
{
    struct WindowConfig
    {
        std::string Title;
        int Width;
        int Height;

        WindowConfig(const std::string& title = "Nyl Engine",
                    int width = 1280,
                    int height = 720)
            : Title(title), Width(width), Height(height) {}
    };

#pragma region vert/shad
    static const struct
    {
        float x, y;
        float r, g, b;
    } vertices[3] =
    {
        { -0.6f, -0.4f, 1.f, 0.f, 0.f },
        {  0.6f, -0.4f, 0.f, 1.f, 0.f },
        {   0.f,  0.6f, 0.f, 0.f, 1.f }
    };

    static const char* vertex_shader_text =
        "#version 110\n"
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

    static const char* fragment_shader_text =
        "#version 110\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";
#pragma endregion
    class Window {
    public:

        WindowConfig m_Config;

        GLuint vertex_buffer;
        GLuint vertex_shader;
        GLuint fragment_shader;
        GLuint program;
        GLint mvp_location;
        GLint vpos_location;
        GLint vcol_location;

        mat4x4 m;
        mat4x4 p;
        mat4x4 mvp;

        Window(int width, int height, const std::string& title);
        ~Window();

        bool Init();
        void SetShaders();
        void Update();
        bool ShouldClose() const;

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void error_callback(int error, const char* description);

        //static const struct
        //{
        //    float x, y;
        //    float r, g, b;
        //} vertices[];

        //static const char* vertex_shader_text;

        //static const char* fragment_shader_text;
    private:
        GLFWwindow* window;
        int width;
        int height;
        std::string title;
        WindowConfig config;
    };

}
