#include "window.h"

#include <glm/glm.hpp>

#include <memory>

#include "log.h"

#include "KeyEvent.h"
#include "MouseEvent.h"
#include "input.h"
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

        // set the event callback functions
        SetEventFunctionCallbacks();
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // set the window user pointer to the Window instance
        glfwSetWindowUserPointer(window, this);

        // glfw compile version
        NYL_CORE_TRACE("Compiled against: {0} {1} {2}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_MINOR);
        // glfw runtime version
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        NYL_CORE_TRACE("GLFW Runtime ver: {0} {1} {2}", major, minor, revision);
}
void Window::SetEventFunctionCallbacks()
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                std::unique_ptr<nyl::KeyPressedEvent> event = std::make_unique<nyl::KeyPressedEvent>((KeyCode)key);
                nyl::EventDispatcher::AddEvent(std::move(event));
            }
        });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (action == GLFW_PRESS)
            {
                std::unique_ptr<nyl::MouseButtonPressedEvent> event = std::make_unique<nyl::MouseButtonPressedEvent>(button, (float)xpos, (float)ypos);
                nyl::EventDispatcher::AddEvent(std::move(event));
            }
            else if (action == GLFW_RELEASE)
            {
                std::unique_ptr<nyl::MouseButtonReleasedEvent> event = std::make_unique<nyl::MouseButtonReleasedEvent>(button, (float)xpos, (float)ypos);
                nyl::EventDispatcher::AddEvent(std::move(event));
            }
        });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
        {
            std::unique_ptr<nyl::MouseMovedEvent> event = std::make_unique<nyl::MouseMovedEvent>((float)xpos, (float)ypos);
            nyl::EventDispatcher::AddEvent(std::move(event));
        });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            std::unique_ptr<nyl::MouseScrolledEvent> event = std::make_unique<nyl::MouseScrolledEvent>((float)xoffset, (float)yoffset);
            nyl::EventDispatcher::AddEvent(std::move(event));
        });
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



void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Window::error_callback(int error, const char* description) {
    NYL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}
