#include "Window.h"
#include<stb/stb_image.h>
//#include "Texture.h"
#include "glm/glm.hpp"
#include<filesystem>
namespace fs = std::filesystem;
namespace Nyl
{

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
#pragma region AAPL

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#pragma endregion
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
        Cleanup();
    }

    bool Window::Init()
    {

        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        shader = new Shader("D:/gitHub/nyl/Nyl/Shaders/default.vert", "D:/gitHub/nyl/Nyl/Shaders/default.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = 
        {
            // positions         // colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,// lower left corner
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f,// upper left corner
         0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,// upper right corner
         0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	  1.0f, 0.0f // lower right corner
        };
        GLuint indices[]=
        {
            0, 2, 1,                                  // lower left triangle
            0, 3, 2,                                  // lower right triangle          
        };
        vao = new VAO();
        vao->Bind();
        // generates Vertex Buffer Object and links it to vertices
        vbo =new VBO(vertices, sizeof(vertices));
        // Generates Element Buffer Object and links it to indices
        ebo = new EBO(indices, sizeof(indices));


        // links VBO attributes such as coordinates and colors to VAO
        vao->LinkVBO(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
        vao->LinkVBO(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        vao->LinkVBO(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        // unbind all to prevent accidentaly modifying them
        vao->Unbind();
        vbo->Unbind();
        ebo->Unbind();

        // gets id of uniform called scale
        scaleID = glGetUniformLocation(shader->ID, "scale");

        ////Texture
        //int widthImg, heightImg, numColCh;
        //stbi_set_flip_vertically_on_load(true);
        ////unsigned char* bytes = stbi_load("../resources/chikboy_idle_0.png", &widthImg, &heightImg, &numColCh, 0);
        //unsigned char* bytes = stbi_load("D:/gitHub/nyl/Nyl/resources/chikboy_idle_0.png", &widthImg, &heightImg, &numColCh, 0);

        std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
        std::string texPath = "\\resources\\";
        auto result = (parentDir + texPath + "chikboy_idle_0.png");
        NYL_CORE_INFO("LOADING {0}", result);
        m_texture = new Texture("D:/gitHub/nyl/Nyl/resources/chikboy_idle_0.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        m_texture->texUnit(*shader, "tex0", 0);


#pragma region old-code
        //glGenTextures(1, &texture);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture);



        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg,0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
        //glGenerateMipmap(GL_TEXTURE_2D);

        //stbi_image_free(bytes);
        //glBindTexture(GL_TEXTURE_2D, 0);

        //GLuint tex0Uni = glGetUniformLocation(shader->ID,"tex0");
        //shader->use();
        //glUniform1i(tex0Uni, 0);
#pragma endregion
        return true;
    }


    // render loop
    // -----------
    void Window::Update()
    {

        while (!glfwWindowShouldClose(window))
        {
            // Specify the color of the background
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            // Clean the back buffer and assign the new color to it
            glClear(GL_COLOR_BUFFER_BIT);
            // Tell OpenGL which Shader Program we want to use
            shader->use();
            // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
            glUniform1f(scaleID, 0.5f);
            // bind texture
            glBindTexture(GL_TEXTURE_2D, texture);
            // Binds texture so that is appears in rendering
            m_texture->Bind();
            // Bind the VAO so OpenGL knows to use it
            vao->Bind();
            // Draw primitives, number of indices, datatype of indices, index of indices
            glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
            // Swap the back buffer with the front buffer
            glfwSwapBuffers(window);
            // Take care of all GLFW events
            glfwPollEvents();
        }
        Cleanup();
    }
    void Window::Cleanup()
    {
        // delete all objects we created
        vao->Delete();
        vbo->Delete();
        ebo->Delete();
        glDeleteTextures(1, &texture);
        shader->Delete();
        // Delete window before ending the program
        glfwDestroyWindow(window);
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
}
