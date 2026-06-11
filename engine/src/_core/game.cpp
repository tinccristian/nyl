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

    // Embedded batch-renderer shader (no external file needed).
    static const char* kBatchVertexShader = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
out vec2 vUV;
out vec4 vColor;
uniform mat4 projection;
uniform mat4 view;
void main()
{
    vUV = aUV;
    vColor = aColor;
    gl_Position = projection * view * vec4(aPos, 0.0, 1.0);
}
)";

    static const char* kBatchFragmentShader = R"(#version 330 core
in vec2 vUV;
in vec4 vColor;
out vec4 FragColor;
uniform sampler2D uTex;
void main()
{
    FragColor = texture(uTex, vUV) * vColor;
}
)";

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

        // batched sprite renderer shader, embedded so the engine has no runtime
        // file dependency (the projection/view are set per-frame by RenderSystem)
        ResourceManager::LoadShaderSource(kBatchVertexShader, kBatchFragmentShader, "batch");

        Init();
    }

    void Game::setWindowTitle(const std::string& title)
    {
        glfwSetWindowTitle(window.getGLFWwindow(), title.c_str());
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