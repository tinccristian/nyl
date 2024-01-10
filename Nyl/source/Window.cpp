#include "Window.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include "glm/glm.hpp"

namespace Nyl
{
    //Shader
    Shader shader;
    VAO vao;
    VBO vbo;
    EBO ebo;

    // Vertices coordinates
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1 // Upper triangle
    };


    Window::Window(int width, int height, const std::string& title)
        : window(nullptr), width(width), height(height), title(title) {}

    Window::~Window() 
    {
        vao.Delete();
        vbo.Delete();
        ebo.Delete();
        shader.Delete();
        glfwDestroyWindow(window);
        glfwTerminate();

    }

    bool Window::Init() 
    {
        //Set the error callback to use our Log system
        glfwSetErrorCallback(error_callback);

        //Initialize GLFW
        if (!glfwInit()) 
        {
            NYL_CORE_ERROR("Failed to init glfw!");
            return false;
        }

        //Tell GLFW what version of OpenGL we are using
        //2.0
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        // Tell GLFW we are using the CORE profile
        // So that means we only have the modern functions
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) 
        {
            NYL_CORE_ERROR("Failed to create a window!");
            glfwTerminate();
            return false;
        }

        glfwSetKeyCallback(window, key_callback);

        // Introduce the window into the current context
        glfwMakeContextCurrent(window);
        //Load GLAD so it configures OpenGL
        gladLoadGL();

        // Specify the viewport of OpenGL in the Window
        // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
        glViewport(0, 0, width, height);


        // Generates shaders
        Shader shader("D:/gitHub/nyl/Nyl/Shaders/default.vert", "D:/gitHub/nyl/Nyl/Shaders/default.frag");
        if (shader.ID != 0) {
            NYL_CORE_ERROR("Shader is not valid!");
        }

        // Generates Vertex Array Object and binds it
        vao.Bind();


        // Take care of all the light related things
        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

        // Generates Vertex Buffer Object and links it to vertices
        vbo = VBO(vertices, sizeof(vertices));
        if (vbo.ID != 0) {
            NYL_CORE_ERROR("VBO is not valid!");
        }
        // Generates Element Buffer Object and links it to indices
        ebo =EBO(indices, sizeof(indices));
        if (ebo.ID != 0) {
            NYL_CORE_ERROR("EBO is not valid!");
        }

        // Links VBO to VAO
        vao.LinkVBO(vbo, 0);
        if (vao.ID != 0) {
            NYL_CORE_ERROR("VAO is not valid!");
        }
        // Unbind all to prevent accidentally modifying them
        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            // Handle or log the error
            NYL_CORE_ERROR("OpenGL error: {0}", error);
        }
        // Validate that VBO, EBO, and VAO setup is successful
        if (vao.ID != 0 || vbo.ID != 0 || ebo.ID != 0) {
            NYL_CORE_ERROR("VBO, EBO, or VAO setup failed!");
            return false;
        }
        return true;
    }

    void Window::Update() 
    {
        //Clear previous error
        glGetError();
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shader.Activate();
        // Bind the VAO so OpenGL knows to use it
        vao.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            // Handle or log the error
            NYL_CORE_ERROR("OpenGL error: {0}", error);
        }
    }

    bool Window::ShouldClose() const 
    {
        return glfwWindowShouldClose(window);
    }

    bool ValidateOpenGLObjects(const Shader& shader, const VBO& vbo, const EBO& ebo, const VAO& vao) {
        bool valid = true;









        return valid;
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
