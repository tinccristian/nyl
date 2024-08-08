#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

//nyl includes
#include "core.h"
#include "log.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
//#include "GameObject.h"

namespace nyl 
{

    class NYL_API Game 
    {
    public:
        // constructors and destructor
        Game(int width, int height, const std::string& title);
        ~Game();

        // initialization
        void init();
        void configureOpenGL();
        void setupImGui();

        // functions to be implemented by the user
        virtual void Init() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Render(float deltaTime) = 0;
        virtual void ProcessInput(float deltaTime) = 0;
        virtual void Quit() = 0;

        // utils
        static float getDeltaTime() { return deltaTime; }
        // main loop
        void run();
        

    private:
        Window window;
        static float deltaTime;
        // shader and texture
        void updateFPS(int& frameCount, float& totalTime);
    };
}