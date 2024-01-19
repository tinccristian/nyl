#include "glm/glm.hpp"

#include "Game.h"
#include "EntityManager.h"


namespace Nyl
{
    GLFWwindow* Game::getWindow()
    {
        return Nyl::Game::window;
    }
    GameObject* Game::getPlayer()
    {
        return Nyl::Game::Player;
    }
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Game::Game(int width, int height, const std::string& title)
        : window(nullptr), width(width), height(height), title(title)
    {
        initialize_glfw_window();
    }

    Game::~Game()
    {
        EntityManager::Clear();
        glfwTerminate();
    }
    void Game::initialize_glfw_window()
    {
        // glfw initialize and configure
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // set the error callback to use our Log system
        glfwSetErrorCallback(error_callback);
#pragma region AAPL

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#pragma endregion
        // no resize
        glfwWindowHint(GLFW_RESIZABLE, false);
        // glfw window creation
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);    //fullscreen    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
        if (window == NULL)
        {
            NYL_CORE_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);



        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            NYL_CORE_ERROR("Failed to initialize GLAD");
            return;
        }

        // set the callback functions, should come from the APP, but for now engine specific 
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);

        // set the window user pointer to the Window instance
        glfwSetWindowUserPointer(window, this);

        // glfw compile version
        NYL_CORE_TRACE("Compiled against: {0} {1} {2}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_MINOR);
        // glfw runtime version
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        NYL_CORE_TRACE("GLFW Runtime ver: {0} {1} {2}", major, minor, revision);
    }

    void Game::init()
    {
        NYL_CORE_INFO("init");


        // OpenGL configuration
        glViewport(0, 0, width, height);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // load shaders
        EntityManager::LoadShader("D:/gitHub/nyl/Nyl/Shaders/sprite.vert", "D:/gitHub/nyl/Nyl/Shaders/sprite.frag", nullptr, "sprite");

        // configure shaders
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);// to be abstracted to app
        EntityManager::GetShader("sprite").use().set_int("sprite", 0);
        EntityManager::GetShader("sprite").set_mat4("projection", projection);
        // load textures
        Init(); //make user init his textures
    }
    void bindJoystick()
    {
        

    }
    void Game::update()
    {
        //NYL_CORE_TRACE("update");
        // get delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        ////////////////////////////////////
    //     joystick = glfwJoystickPresent(GLFW_JOYSTICK_1);

    //    //axes ~ should be 6 for regular controller: leftAnalog axis x,y, rightAnalog axis x,y, left/right triggers (LT) 1 axis each
    //    int axesCount;
    //    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
    //    NYL_CORE_INFO("#axes available: {0}", axesCount);

    //    // buttons
    //     const unsigned char* buttons;
    //    int buttonCount;
    //    buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

    //    //info
    //    NYL_CORE_INFO("{0} is connected, it has {1} axes available, {2} buttons", glfwGetJoystickName(GLFW_JOYSTICK_1), axesCount,buttonCount);


    //        //joystick
    //        if (1 == joystick1)
    //        {
    //            NYL_CORE_TRACE("\n\n\n\n\n\n\n\n\n\n\n\n\n\nLeft stick X Axis: {0}", axes[0]);
    //            NYL_CORE_TRACE("Left stick Y Axis: {0}", axes[1]);
    //            NYL_CORE_TRACE("Right stick X Axis: {0}", axes[2]);
    //            NYL_CORE_TRACE("Right stick Y Axis: {0}", axes[5]);
    //            NYL_CORE_TRACE("Left Trigger/L2: {0}", axes[4]);
    //            NYL_CORE_TRACE("Right Trigger/R2: {0}", axes[3]);


    //            if (GLFW_PRESS == buttons[1])
    //            {
    //                NYL_CORE_TRACE("X button pressed");
    //            }
	//         }
        ///////////////////////////////
        ProcessInput(0);// Antares foo

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Update(0);
        // game update
        glfwSwapBuffers(window);
    }
    void Game::run()
    {
        init();
        while (!should_close())
        {
            update();
        }
    }
    void Game::cleanup()
    {
        NYL_CORE_INFO("cleanup");
    }
#pragma region oldcode
    //    // input
    //    // -----
    //    
    //    //set the mappings
    //    joystick = glfwJoystickPresent(GLFW_JOYSTICK_1);
    //    std::string mappings = load_file_contents("D:/gitHub/nyl/Nyl/thirdparty/gamecontrollerdb.txt");
    //    glfwUpdateGamepadMappings(mappings.c_str());

    //    //axes ~ //should be 6 for regular controller: leftAnalog axis x,y, rightAnalog axis x,y, left/right triggers (LT) 1 axis each
    //    int axesCount;
    //    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
    //    NYL_CORE_INFO("#axes available: {0}", axesCount);

    //    // buttons
    //    int buttonCount;
    //    buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

    //    //info
    //    NYL_CORE_INFO("{0} is connected, it has {1} axes available, {2} buttons", glfwGetJoystickName(GLFW_JOYSTICK_1), axesCount,buttonCount);


    //        //joystick
    //        if (1 == joystick)
    //        {
    //            //NYL_CORE_TRACE("\n\n\n\n\n\n\n\n\n\n\n\n\n\nLeft stick X Axis: {0}", axes[0]);
    //            //NYL_CORE_TRACE("Left stick Y Axis: {0}", axes[1]);
    //            //NYL_CORE_TRACE("Right stick X Axis: {0}", axes[2]);
    //            //NYL_CORE_TRACE("Right stick Y Axis: {0}", axes[5]);
    //            //NYL_CORE_TRACE("Left Trigger/L2: {0}", axes[4]);
    //            //NYL_CORE_TRACE("Right Trigger/R2: {0}", axes[3]);


    //            if (GLFW_PRESS == buttons[1])
    //            {
    //                NYL_CORE_TRACE("X button pressed");
    //            }
#pragma endregion
    // function to check if the joystick is connected
    int is_joystick_connected()
	{
		return glfwJoystickPresent(GLFW_JOYSTICK_1);
	}

	}

 void Game::toggle_polygon_mode()
{
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);

        if (polygonMode[0] == GL_FILL)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//
            NYL_CORE_TRACE("Wireframe mode set.");
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//
            NYL_CORE_TRACE("Line mode set.");
        }
}
bool Game::should_close() const
    {
        return glfwWindowShouldClose(window);
    }
void Game::framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}
// void Game::process_input()//press and hold events ~~ not used for now
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
// }
//INPUT -> to be moved to separate header
void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //press key events
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        toggle_polygon_mode();
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        NYL_CORE_WARN("going up");
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        NYL_CORE_WARN("going left");
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        NYL_CORE_WARN("going down");
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        NYL_CORE_WARN("going right");
}

 void Game::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
     NYL_CORE_INFO("x: {0}, y: {1}", xpos, ypos);
}

void Game::error_callback(int error, const char* description)
{
    NYL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}
