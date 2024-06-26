#include "window.h"

#include "log.h"


#include <glm/glm.hpp>

Window::Window(int width, int height, const std::string& title)
    : window(nullptr), width(width), height(height), title(title) {
    initializeGLFWwindow();
}

Window::~Window() {
    glfwTerminate();
}

GLFWwindow* Window::getGLFWwindow() const {
    return window;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::initializeGLFWwindow() 
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

		//VSYNC off
		glfwSwapInterval(0);

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

 void Window::toggle_polygon_mode()
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

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //press key events
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

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // // Convert from GLFW coordinates to world coordinates
    // Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    // double worldX = xpos - game->width / 2.0;
    // double worldY = game->height - ypos - game->height / 2.0;

    // NYL_CORE_INFO("World coordinates - x: {0}, y: {1}", worldX, worldY);
    // //NYL_CORE_INFO("x: {0}, y: {1}", xpos, ypos);
}
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Window::error_callback(int error, const char* description) {
    NYL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        
        NYL_CORE_INFO("Glfw coordinates - x: {0}, y: {1}", xpos, ypos);
        // Get the Window instance from the GLFWwindow
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        // Convert from GLFW coordinates to world coordinates
        double worldX = xpos - win->width / 2.0;
        double worldY = win->height - ypos - win->height / 2.0;

        //NYL_CORE_INFO("World coordinates - x: {0}, y: {1}", worldX, worldY);
    }
}