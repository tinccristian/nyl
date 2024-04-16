#include "game.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <thread>

#include "utils.h"
#include "resource_manager.h"
#include "system_renderer.h"

namespace nyl
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    const int targetFPS = 1000;

    Game::Game(int width, int height, const std::string& title)
        : window(width, height, title){}

    Game::~Game()
    {
        ResourceManager::Clear();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Game::setupImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        ImGui_ImplGlfw_InitForOpenGL(window.getGLFWwindow(), true);
        ImGui_ImplOpenGL3_Init();
    }

    void Game::configureOpenGL()
    {
        glViewport(0, 0, window.width, window.height);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Game::init()
    {
        NYL_CORE_INFO("init");
        setupImGui();
        configureOpenGL();
        // load default shaders
        ResourceManager::LoadShader(getFullPath("../../resources/shaders/sprite.vert").c_str(), getFullPath("../../resources/shaders/sprite.frag").c_str(), nullptr, "sprite");
        ResourceManager::LoadShader(getFullPath("../../resources/shaders/debug.vert").c_str(), getFullPath("../../resources/shaders/debug.frag").c_str(), nullptr, "debug");
        
        // configure shaders
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window.width), static_cast<float>(window.height), 0.0f, -1.0f, 1.0f);
        ResourceManager::GetShader("sprite")->use().set_int("sprite", 0);
        ResourceManager::GetShader("sprite")->set_mat4("projection", projection);

        ResourceManager::GetShader("debug")->use().set_int("debug", 0);
        ResourceManager::GetShader("debug")->set_mat4("projection", projection);

        Init();
    }

    void Game::updateFPS(int& frameCount, float& totalTime)
    {
        frameCount++; totalTime += deltaTime;
        if (totalTime >= 1.0f) {
            int averageFPS = static_cast<int>(frameCount / totalTime);
            std::string newTitle = window.title + " - FPS: " + std::to_string(averageFPS);
            glfwSetWindowTitle(window.getGLFWwindow(), newTitle.c_str());
            frameCount = 0; totalTime = 0.0f;
        }
    }

    void Game::run()
    {
        init();
        const float targetFrameTime = 1.0f / targetFPS;
        int frameCount = 0;
        float totalTime, frameTime, sleepTime = 0.0f;
        
        while (!window.shouldClose())
        {
            float frameStart = glfwGetTime();

            // get delta time
            deltaTime = frameStart - lastFrame;
            lastFrame = frameStart;

            Update(deltaTime);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Render();

            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow(); // Show demo window! :)
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window.getGLFWwindow());

            frameTime = glfwGetTime() - frameStart; // time taken for this frame

            if (frameTime < targetFrameTime) //lock the framerate
            {
                sleepTime = targetFrameTime - frameTime;
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
            }
            updateFPS(frameCount, totalTime);
        }
    }
} // namespace nyl