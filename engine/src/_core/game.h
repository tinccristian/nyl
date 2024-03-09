#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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
        virtual void Init() = 0;

        // update
        void update();
        virtual void Update(float deltaTime) = 0;

        // input processing from app
        virtual void ProcessInput(float deltaTime) = 0;
        
        // cleanup
        virtual void Quit() = 0;

        // main loop
        void run();

    protected:
        // struct for a point
        struct point {
            float x = 60.0f;
            float y = 280.0f;
        };

    // shader and texture
    ShaderComponent* shader;
    TextureComponent* m_texture;


    private:
        Window window;
    };

}