#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

//nyl includes
#include "core.h"
#include "core_log.h"
#include "component_shader.h"
#include "component_texture.h"
//#include "GameObject.h"

namespace Nyl 
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
        void cleanup();
        virtual void Quit() = 0;

        // main loop
        void run();

        // accessors
        GLFWwindow* getWindow();
        // GameObject* getPlayer();


        // static function for toggling polygon mode
        static void toggle_polygon_mode();

        // static callback functions
        static void framebuffer_size_callback(GLFWwindow*, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void error_callback(int error, const char* description);
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    protected:
        // struct for a point
        struct point {
            float x = 60.0f;
            float y = 280.0f;
        };

    // player ptr
    //GameObject* Player;
    // window properties
    GLFWwindow* window;
    int width;
    int height;
    std::string title;

    // shader and texture
    ShaderComponent* shader;
    TextureComponent* m_texture;


    private:
        // private helper hunction for GLFW window initialization
        void initialize_glfw_window();
        // private GLFW window util function
        bool should_close() const;
    };

}