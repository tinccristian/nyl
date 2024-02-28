#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "core_game.h"
#include "resource_manager.h"
#include "system_renderer.h"
namespace Nyl
{
    GLFWwindow* Game::getWindow()
    {
        return Nyl::Game::window;
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
        ResourceManager::Clear();
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
        glfwSetMouseButtonCallback(window, mouse_button_callback);

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
        ResourceManager::LoadShader("D:/gitHub/nyl/engine/resources/shaders/sprite.vert", "D:/gitHub/nyl/engine/resources/shaders/sprite.frag", nullptr, "sprite");
        ResourceManager::LoadShader("D:/gitHub/nyl/engine/resources/shaders/debug.vert", "D:/gitHub/nyl/engine/resources/shaders/debug.frag", nullptr, "debug");

        // configure shaders TODO::Abstract this garbage
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);// to be abstracted to app
        ResourceManager::GetShader("sprite")->use().set_int("sprite", 0);
        ResourceManager::GetShader("sprite")->set_mat4("projection", projection);

        ResourceManager::GetShader("debug")->use().set_int("debug", 0);
        ResourceManager::GetShader("debug")->set_mat4("projection", projection);
        // load textures
        Init(); //make user init his textures
    }
    #include <chrono>
    #include <thread>

    void Game::update()
    {
        //NYL_CORE_TRACE("update");
        // get delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //ProcessInput(0);// Antares foo


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Update(deltaTime);
        glfwPollEvents();
        // game update
        glfwSwapBuffers(window);
    }
void Game::run()
{
    init();
    const int targetFPS = 400; // Change this to 400
    const float targetFrameTime = 1.0f / targetFPS;
    int frameCount = 0;
    float totalTime = 0.0f;

    while (!should_close())
    {
        float startFrame = glfwGetTime();

        update();

        float endFrame = glfwGetTime();
        float frameTime = endFrame - startFrame; // time taken for this frame

        // if (frameTime < targetFrameTime)
        // {
        //     float sleepTime = targetFrameTime - frameTime;
        //     std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
        // }

        frameCount++;
        totalTime += frameTime;

        if (totalTime >= 1.0f)
        {
            int averageFPS = (int)(frameCount / totalTime);
            std::string newTitle = title + " - FPS: " + std::to_string(averageFPS);
            glfwSetWindowTitle(window, newTitle.c_str());
            // reset frameCount and totalTime
            frameCount = 0;
            totalTime = 0.0f;
        }
    }
}
void Game::cleanup()
{
    NYL_CORE_INFO("cleanup");
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
#pragma region callbacks
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
    // // Convert from GLFW coordinates to world coordinates
    // Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    // double worldX = xpos - game->width / 2.0;
    // double worldY = game->height - ypos - game->height / 2.0;

    // NYL_CORE_INFO("World coordinates - x: {0}, y: {1}", worldX, worldY);
    // //NYL_CORE_INFO("x: {0}, y: {1}", xpos, ypos);
}
void Game::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
        // Convert from GLFW coordinates to world coordinates

        double worldX = xpos - game->width / 2.0;
        double worldY = game->height - ypos - game->height / 2.0;

        NYL_CORE_INFO("World coordinates - x: {0}, y: {1}", worldX, worldY);
    }
}
void Game::error_callback(int error, const char* description)
{
    NYL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}
#pragma endregion

} // namespace Nyl