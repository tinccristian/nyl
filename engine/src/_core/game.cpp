#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "game.h"
#include "resource_manager.h"
#include "system_renderer.h"

#include "utils.h"

namespace nyl
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Game::Game(int width, int height, const std::string& title)
        : window(width, height, title){}

    Game::~Game()
    {
        ResourceManager::Clear();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Game::init()
    {
        NYL_CORE_INFO("init");

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window.getGLFWwindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();

        // OpenGL configuration
        glViewport(0, 0, window.width, window.height);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // load default shaders
        ResourceManager::LoadShader(getFullPath("../../resources/shaders/sprite.vert").c_str(), getFullPath("../../resources/shaders/sprite.frag").c_str(), nullptr, "sprite");
        ResourceManager::LoadShader(getFullPath("../../resources/shaders/debug.vert").c_str(), getFullPath("../../resources/shaders/debug.frag").c_str(), nullptr, "debug");
        
        // configure shaders TODO::Abstract this garbage
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window.width), static_cast<float>(window.height), 0.0f, -1.0f, 1.0f);// to be abstracted to app?
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
        //ProcessInput(0);// platformer foo


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Update(deltaTime);
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)
        // game update
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window.getGLFWwindow());
    }
void Game::run()
{
    init();
    const int targetFPS = 400; // Change this to 400
    const float targetFrameTime = 1.0f / targetFPS;
    int frameCount = 0;
    float totalTime = 0.0f;

    while (!window.shouldClose())
    {
        float startFrame = glfwGetTime();

        update();

        float endFrame = glfwGetTime();
        float frameTime = endFrame - startFrame; // time taken for this frame

        // if (frameTime < targetFrameTime) //lock the framerate
        // {
        //     float sleepTime = targetFrameTime - frameTime;
        //     std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
        // }

        frameCount++;
        totalTime += frameTime;

        if (totalTime >= 1.0f)
        {
            int averageFPS = (int)(frameCount / totalTime);
            std::string newTitle = window.title + " - FPS: " + std::to_string(averageFPS);
            glfwSetWindowTitle(window.getGLFWwindow(), newTitle.c_str());
            // reset frameCount and totalTime
            frameCount = 0;
            totalTime = 0.0f;
        }
    }
}

} // namespace nyl