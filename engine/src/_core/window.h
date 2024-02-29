#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    GLFWwindow* getGLFWwindow() const;
    bool shouldClose() const;
    void swapBuffers();

    int width;
    int height;
    std::string title;
private:
    GLFWwindow* window;

    void initializeGLFWwindow();

    // static function for toggling polygon mode
    static void toggle_polygon_mode();
    // static callback functions
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void framebuffer_size_callback(GLFWwindow*, int width, int height);
    static void error_callback(int error, const char* description);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};