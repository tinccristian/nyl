#include "Window.h"


#include "glm/glm.hpp"

namespace Nyl
{

#pragma region variables
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

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";
#pragma endregion

#pragma region Window constructor
    Window::Window(int width, int height, const std::string& title)
        : window(nullptr), width(width), height(height), title(title) 
    {
        //Set the error callback to use our Log system
        glfwSetErrorCallback(error_callback);

        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }
        // Set the window user pointer to the Window instance
        glfwSetKeyCallback(window, key_callback);
        glfwSetWindowUserPointer(window, this);

    }
#pragma endregion
    Window::~Window() 
    {
        vao->Delete();
        vbo->Delete();
        ebo->Delete();
        //shader->Delete();
        glfwDestroyWindow(window);
        glfwTerminate();

    }

    bool Window::Init()
    {

        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        shader = new Shader("D:/gitHub/nyl/Nyl/Shaders/default.vert", "D:/gitHub/nyl/Nyl/Shaders/default.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
        float vertices[] = {
            // positions         // colors
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        // glBindVertexArray(0);
        // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)


        // uncomment this call to draw in wireframe polygons.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



        return true;
    }
    // render loop
    // -----------
    void Window::Update()
    {

        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // render the triangle
            shader->use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        Cleanup();
    }
    void Window::Cleanup()
    {
        // optional: de-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        //glDeleteProgram(shader);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }
#pragma region utils
    void Window::framebuffer_size_callback(GLFWwindow*, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
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
     void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //press key events
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
            togglePolygonMode();

    }
     void Window::error_callback(int error, const char* description)
    {
         NYL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }
     // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
     // ---------------------------------------------------------------------------------------------------------
     void Window::processInput(GLFWwindow* window)                          //press and hold events
     {
         if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
             glfwSetWindowShouldClose(window, true);
     }
#pragma endregion
#pragma region comments
            //// input
            //// -----
            //processInput(window);

            //// render
            //// ------
            //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            //glClear(GL_COLOR_BUFFER_BIT);

            //glUseProgram(shaderProgram);
            //// draw first triangle using the data from the first VAO
            //glBindVertexArray(VAOs[0]);
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            //// then we draw the second triangle using the data from the second VAO
            //glBindVertexArray(VAOs[1]);
            //glDrawArrays(GL_TRIANGLES, 0, 3);

            //// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            //// -------------------------------------------------------------------------------
            //glfwSwapBuffers(window);
            //glfwPollEvents();

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        //glDeleteVertexArrays(2, VAOs);
        //glDeleteBuffers(2, VBOs);
        //glDeleteProgram(shaderProgram);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        // Generates Shader object using shaders defualt.vert and default.frag
        //Shader shaderProgram("D:/gitHub/nyl/Nyl/Shaders/default.vert", "D:/gitHub/nyl/Nyl/Shaders/default.frag");



        //// Generates Vertex Array Object and binds it
        //VAO VAO1;
        //VAO1.Bind();

        //// Generates Vertex Buffer Object and links it to vertices
        //VBO VBO1(vertices, sizeof(vertices));
        //// Generates Element Buffer Object and links it to indices
        //EBO EBO1(indices, sizeof(indices));

        //// Links VBO to VAO
        //VAO1.LinkVBO(VBO1, 0);
        //// Unbind all to prevent accidentally modifying them
        //VAO1.Unbind();
        //VBO1.Unbind();
        //EBO1.Unbind();



        //// Main while loop
        //while (!glfwWindowShouldClose(window))
        //{
        //    // Specify the color of the background
        //    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        //    // Clean the back buffer and assign the new color to it
        //    glClear(GL_COLOR_BUFFER_BIT);
        //    // Tell OpenGL which Shader Program we want to use
        //    shaderProgram.Activate();
        //    // Bind the VAO so OpenGL knows to use it
        //    VAO1.Bind();
        //    // Draw primitives, number of indices, datatype of indices, index of indices
        //    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        //    // Swap the back buffer with the front buffer
        //    glfwSwapBuffers(window);
        //    // Take care of all GLFW events
        //    glfwPollEvents();
        //}



        //// Delete all the objects we've created
        //VAO1.Delete();
        //VBO1.Delete();
        //EBO1.Delete();
        //shaderProgram.Delete();
        //// Delete window before ending the program
        //glfwDestroyWindow(window);
        //// Terminate GLFW before ending the program
        //glfwTerminate();
        //return 0;
    //void Window::Update() 
    //{
    //    //Clear previous error
    //    glGetError();
    //    // Specify the color of the background
    //    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    //    // Clean the back buffer and assign the new color to it
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    // Tell OpenGL which Shader Program we want to use
    //    shader->Activate();
    //    // Bind the VAO so OpenGL knows to use it
    //    vao->Bind();
    //    // Draw primitives, number of indices, datatype of indices, index of indices
    //    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    //    // Swap the back buffer with the front buffer
    //    glfwSwapBuffers(window);
    //    // Take care of all GLFW events
    //    glfwPollEvents();

    //    GLenum error = glGetError();
    //    if (error != GL_NO_ERROR) {
    //        // Handle or log the error
    //        NYL_CORE_ERROR("OpenGL error: {0}", error);
    //    }
    //}
#pragma endregion
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
}
