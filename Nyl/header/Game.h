#ifndef GAME_H
#define GAME_H

//#include "IGAME.h"
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

    class Game{
    public:


        Game(int width, int height, std::string& title);
        ~Game();
        void init() ;
        void update() ;
        void cleanup() ;
        bool ShouldClose() const;

        GLFWwindow* getWindow();
        void processInput();
        bool getWireframeMode() { return isWireframeMode; }
        static void togglePolygonMode();
#pragma region callbacks

        static void framebuffer_size_callback(GLFWwindow*, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void error_callback(int error, const char* description);

#pragma endregion

    protected:
        // window
        GLFWwindow* window;
        bool isWireframeMode=false;
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

    };

}
#endif