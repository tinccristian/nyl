#include "Window.h"

namespace Nyl
{



    Window::Window(int width, int height, const std::string& title)
        : window(nullptr), width(width), height(height), title(title) {}

    Window::~Window() 
    {
        glfwDestroyWindow(window);
        glfwTerminate();

    }

    bool Window::Init() 
    {
        glfwSetErrorCallback(error_callback);

        if (!glfwInit()) 
        {
            NYL_CORE_ERROR("Failed to init glfw!");
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) 
        {
            NYL_CORE_ERROR("Failed to create a window!");
            glfwTerminate();
            return false;
        }

        glfwSetKeyCallback(window, key_callback);
        glfwMakeContextCurrent(window);
        gladLoadGL();
        glfwSwapInterval(1);

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        mvp_location = glGetUniformLocation(program, "MVP");
        vpos_location = glGetAttribLocation(program, "vPos");
        vcol_location = glGetAttribLocation(program, "vCol");

        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
            sizeof(vertices[0]), (void*)0);
        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
            sizeof(vertices[0]), (void*)(sizeof(float) * 2));


        return true;
    }

    void Window::Update() 
    {
        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool Window::ShouldClose() const 
    {
        return glfwWindowShouldClose(window);
    }



    //INPUT -> to be moved to separate header
     void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
     void Window::error_callback(int error, const char* description)
    {
         NYL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }
}
