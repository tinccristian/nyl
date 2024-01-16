#include "Game.h"
#include<stb/stb_image.h>
//#include "Texture.h"
#include"Utils.h"
#include "glm/glm.hpp"
#include<filesystem>

namespace fs = std::filesystem;

namespace Nyl
{
    GLFWwindow* Game::getWindow()
    {
        return Nyl::Game::window;
    }

#pragma region Game constructor
    Game::Game(int width, int height, std::string& title)
        : window(nullptr), width(width), height(height), title(title)
    {


        // glfw: initialize and configure
        // ------------------------------
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
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            NYL_CORE_ERROR("Failed to initialize GLAD");
            return;
        }
        // Set the window user pointer to the Window instance
        glfwSetKeyCallback(window, key_callback);
        glfwSetWindowUserPointer(window, this);


        // glfw: Compile version
        NYL_CORE_INFO("Compiled against: {0} {1} {2}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_MINOR);
        // glfw: Runtime version
        int major, minor, revision;
        glfwGetVersion(&major, &minor, &revision);
        NYL_CORE_INFO("GLFW Runtime ver: {0} {1} {2}", major, minor, revision);
        //init();//declared by Antares
    }
#pragma endregion

    Game::~Game()
    {
        //Cleanup();
    }
    int Game::init()
    {
        NYL_CORE_INFO("init");
        //other init stuff
        //Init();
        
        //ApplicationInit(); ~function declared in antares.cpp
        return 1;
    }
    void Game::update()
    {
        NYL_CORE_INFO("update");

        //while (!ShouldClose)
        //{
        //    //Update();
        //    //update some stuff;
        //    //ApplicationUpdate(); ~function declared in antares.cpp
        //}
    }
    void Game::cleanup()
    {
        NYL_CORE_INFO("cleanup");
    }
#pragma region old

    //bool Window::Init()
    //{

    //    // build and compile our shader program
    //    // ------------------------------------
    //    // vertex shader
    //    shader = new Shader("D:/gitHub/nyl/Nyl/Shaders/default.vert", "D:/gitHub/nyl/Nyl/Shaders/default.frag");

    //    // set up vertex data (and buffer(s)) and configure vertex attributes
    //    // ------------------------------------------------------------------
    //    float vertices[] = 
    //    {
    //        // positions         // colors
    //    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,// lower left corner
    //    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     0.0f, 1.0f,// upper left corner
    //     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,// upper right corner
    //     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	  1.0f, 0.0f // lower right corner
    //    };
    //    GLuint indices[]=
    //    {
    //        0, 2, 1,                                  // lower left triangle
    //        0, 3, 2,                                  // lower right triangle          
    //    };
    //    vao = new VAO();
    //    vao->Bind();
    //    // generates Vertex Buffer Object and links it to vertices
    //    vbo =new VBO(vertices, sizeof(vertices));
    //    // Generates Element Buffer Object and links it to indices
    //    ebo = new EBO(indices, sizeof(indices));


    //    // links VBO attributes such as coordinates and colors to VAO
    //    vao->LinkVBO(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    //    vao->LinkVBO(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //    vao->LinkVBO(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //    // unbind all to prevent accidentaly modifying them
    //    vao->Unbind();
    //    vbo->Unbind();
    //    ebo->Unbind();

    //    // gets id of uniform called scale
    //    scaleID = glGetUniformLocation(shader->ID, "scale");

    //    ////Texture
    //    //int widthImg, heightImg, numColCh;
    //    //stbi_set_flip_vertically_on_load(true);
    //    ////unsigned char* bytes = stbi_load("../resources/chikboy_idle_0.png", &widthImg, &heightImg, &numColCh, 0);
    //    //unsigned char* bytes = stbi_load("D:/gitHub/nyl/Nyl/resources/chikboy_idle_0.png", &widthImg, &heightImg, &numColCh, 0);

    //    // texture
    //    // -------
    //    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
    //    std::string texPath = "\\resources\\";
    //    auto result = (parentDir + texPath + "chikboy_idle_0.png");
    //    m_texture = new Texture("D:/gitHub/nyl/Nyl/resources/chikboy_idle_0.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    //    NYL_CORE_INFO("Loading: {0}", result);//"D:/gitHub/nyl/Nyl/resources/chikboy_idle_0.png"
    //    m_texture->texUnit(*shader, "tex0", 0);

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

    //    return true;
    //}


    //// render loop
    //// -----------
    //void Window::Update()
    //{

    //    while (!glfwWindowShouldClose(window))
    //    {

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
    //            //else if (GLFW_RELEASE == buttons[1])
    //            //{
    //            //    NYL_CORE_TRACE("X button released");
    //            //}
    //            if (GLFW_PRESS == buttons[2])
    //            {
    //                NYL_CORE_TRACE("Triangle button pressed");
    //            }
    //            //else if (GLFW_RELEASE == buttons[1])
    //            //{
    //            //    NYL_CORE_TRACE("X button released");
    //            //}
    //            //std::cout << axes[0];
    //        }
    //        else
    //        {
    //            NYL_CORE_WARN("no controller detected");
    //        }
    //        // Specify the color of the background
    //        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    //        // Clean the back buffer and assign the new color to it
    //        glClear(GL_COLOR_BUFFER_BIT);
    //        // Tell OpenGL which Shader Program we want to use
    //        shader->use();
    //        // Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
    //        glUniform1f(scaleID, 0.5f);
    //        // bind texture
    //        glBindTexture(GL_TEXTURE_2D, texture);
    //        // Binds texture so that is appears in rendering
    //        m_texture->Bind();
    //        // Bind the VAO so OpenGL knows to use it
    //        vao->Bind();
    //        // Draw primitives, number of indices, datatype of indices, index of indices
    //        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    //        // Swap the back buffer with the front buffer
    //        glfwSwapBuffers(window);
    //        // Process pending events: https://www.glfw.org/docs/3.3/input_guide.html#gamepad_mapping
    //        glfwPollEvents();               //processes only those events that have already been received and then returns immediately.
    //        //glfwWaitEvents();               //If you only need to update the contents of the window when you receive new input, glfwWaitEvents is a better choice.It puts the thread to sleep until at least one event has been received and then processes all received events. This saves a great deal of CPU cycles and is useful for, for example, editing tools.
    //        //glfwWaitEventsTimeout(0.7);     //If you want to wait for events but have UI elements or other tasks that need periodic updates, glfwWaitEventsTimeout lets you specify a timeout.It puts the thread to sleep until at least one event has been received, or until the specified number of seconds have elapsed. It then processes any received events.
    //        //glfwPostEmptyEvent();           //If the main thread is sleeping in glfwWaitEvents, you can wake it from another thread by posting an empty event to the event queue with glfwPostEmptyEvent.
    //    }
    //    Cleanup();
    //}
    //void Window::Cleanup()
    //{
    //    // delete all objects we created
    //    vao->Delete();
    //    vbo->Delete();
    //    ebo->Delete();
    //    glDeleteTextures(1, &texture);
    //    shader->Delete();
    //    // Delete window before ending the program
    //    glfwDestroyWindow(window);
    //    // glfw: terminate, clearing all previously allocated GLFW resources.
    //    // ------------------------------------------------------------------
    //    glfwTerminate();
    //}
#pragma endregion

 void Game::togglePolygonMode()
{
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);

        if (polygonMode[0] == GL_LINE)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//
            NYL_CORE_INFO("Wireframe mode set.");
        }
        else if (polygonMode[1] == GL_FILL)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//
            NYL_CORE_INFO("Line mode set.");
        }
}
bool Game::ShouldClose() const
    {
        return glfwWindowShouldClose(window);
    }
    bool Game::ValidateOpenGLObjects(const Shader& shader, const VBO& vbo, const EBO& ebo, const VAO& vao) {
        bool valid = true;

        return valid;
    }
    void Game::framebuffer_size_callback(GLFWwindow*, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
    void Game::processInput(GLFWwindow* window)//press and hold events ~~ not used for now
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    //INPUT -> to be moved to separate header
    void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //press key events
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
            togglePolygonMode();

    }
    void Game::error_callback(int error, const char* description)
    {
        NYL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------

}
