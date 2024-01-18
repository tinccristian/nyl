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
    void Game::update(float dt)
    {
        //NYL_CORE_TRACE("update");
        // get delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        process_input();// to be moved to antares as separate foo

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Update();
        // game update
        glfwSwapBuffers(window);
    }
    void Game::run()
    {
        init();
        while (!should_close())
        {
            update(0);
            Update();
        }
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
void Game::process_input()//press and hold events ~~ not used for now
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
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

}
