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
#include "Texture.h"

namespace Nyl
{

    class NYL_API Game{
    public:


        Game(int width, int height, const std::string& title);
        ~Game();
        void init() ;
        void update(float dt) ;
        void cleanup() ;
        bool should_close() const;
        virtual void Init()=0;
        virtual void Update()=0;
        virtual void Quit()=0;
        void run();
        GLFWwindow* getWindow();
        void process_input();
        static void toggle_polygon_mode();
#pragma region callbacks

        typedef struct point {
            float x = 60.0f;
            float y = 280.0f;
        };
        static void framebuffer_size_callback(GLFWwindow*, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void error_callback(int error, const char* description);

#pragma endregion

    protected:
        // window
        GLFWwindow* window;
        int  width;
        int  height;
        std::string title;
        // shader
        Shader*  shader;
        // texture
        Texture* m_texture;
        GLuint   texture;
        GLuint   scaleID;
        // input
        int joystick;
        const unsigned char* buttons;
    private:
        void initialize_glfw_window();
    };

}