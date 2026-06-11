#include "game.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <thread>

#include "utils.h"
#include "resource_manager.h"
#include "system_renderer.h"
#include "component_registry.h"
#include "input.h"
#include "audio.h"

namespace nyl
{
    float Game::deltaTime = 0.0f;
    float lastFrame = 0.0f;
    const int targetFPS = 1000;

    Game::Game(int width, int height, const std::string& title)
        : window(width, height, title){}

    Game::~Game()
    {
        AudioEngine::Shutdown();
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
        // register engine component types so the registry / serialization /
        // editor can introspect them
        ComponentRegistry::RegisterBuiltins();
        // wire up keyboard/mouse input polling
        Input::Init(window.getGLFWwindow());
        // audio (tolerant of headless / no-device environments)
        AudioEngine::Init();
        setupImGui();
        configureOpenGL();
        // load default shaders
        ResourceManager::LoadShader(getFullPath("../../resources/shaders/sprite.vert").c_str(), getFullPath("../../resources/shaders/sprite.frag").c_str(), nullptr, "sprite");
        ResourceManager::LoadShader(getFullPath("../../resources/shaders/debug.vert").c_str(), getFullPath("../../resources/shaders/debug.frag").c_str(), nullptr, "debug");
        // batched sprite renderer shader
        ResourceManager::LoadShader(getFullPath("../../resources/shaders/batch.vert").c_str(), getFullPath("../../resources/shaders/batch.frag").c_str(), nullptr, "batch");
        
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
        // largest delta time we will ever report to a frame; prevents the
        // first-frame spike (lastFrame == 0) and post-stall explosions from
        // launching the simulation.
        const float maxDeltaTime = 0.05f; // 20 FPS floor
        // fixed timestep for physics/collision (framerate-independent)
        const float fixedDeltaTime = 1.0f / 120.0f;
        float physicsAccumulator = 0.0f;
        int frameCount = 0;
        float totalTime = 0.0f, frameTime = 0.0f, sleepTime = 0.0f, frameStart = 0.0f;

        while (!window.shouldClose())
        {
            frameStart = glfwGetTime();

            // get delta time (clamped so a long pause can't blow up physics)
            deltaTime = frameStart - lastFrame;
            lastFrame = frameStart;
            if (deltaTime > maxDeltaTime) { deltaTime = maxDeltaTime; }
            if (deltaTime < 0.0f) { deltaTime = 0.0f; }

            // input first: snapshot previous state, then pump events
            Input::Update();
            glfwPollEvents();

            // engine now drives the full per-frame contract
            ProcessInput(deltaTime);

            // fixed-timestep simulation (physics/collision)
            physicsAccumulator += deltaTime;
            while (physicsAccumulator >= fixedDeltaTime)
            {
                FixedUpdate(fixedDeltaTime);
                physicsAccumulator -= fixedDeltaTime;
            }

            Update(deltaTime);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Render(deltaTime);

            // ImGui frame — content is owned by the app/editor via OnImGui()
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            OnImGui(deltaTime);
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