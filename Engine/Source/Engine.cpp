#include "Engine.hpp"

#include "Assets/Material.hpp"
#include "Assets/Texture.hpp"
#include "DefaultEngineConfig.hpp"
#include "EngineStatics.hpp"

#include <GLFW/glfw3.h>


CEngine::CEngine() {
    GEngine = this;

    Initialize();

    while (IsRunning) {
        MainLoop();
    }

    Shutdown();
}

void CEngine::Initialize() {
    srand(time(nullptr));

    ConfigureEngine();

    GLog = new CLog();
    GLog->Initialize();

    glfwInit();
    CreateWindow();

    Renderer = new CRenderer();
    GAssetManager = new CAssetManager();
    GInputManager = new CInputManager();

    Renderer->DefaultTexture = GetAsset<CTexture>("Engine/Textures/DefaultTexture.png");
    Renderer->DefaultMaterial = GetAsset<CMaterial>("Engine/Materials/SimpleShading.mat");
    Renderer->DefaultWidgetMaterial = GetAsset<CMaterial>("Engine/Materials/Image.mat");

    Log("Engine", ELogLevel::Info, "Loading World: " + GEngineConfig.WorldClass->Name);
    World = std::unique_ptr<CWorld>(GEngineConfig.WorldClass.CreateObject());
    World->BeginPlay();

    previousFrameTime = std::chrono::steady_clock::now();
}

void CEngine::MainLoop() {
    glfwPollEvents();
    if (glfwWindowShouldClose(Window)) {
        IsRunning = false;
    }

    if (PendingWorldClass) {
        Log("Engine", ELogLevel::Info, "Loading World: " + PendingWorldClass->Name);
        World = std::unique_ptr<CWorld>(PendingWorldClass.CreateObject());
        World->BeginPlay();
        PendingWorldClass = nullptr;
    }

    const auto now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - previousFrameTime).count();
    previousFrameTime = now;

    deltaTime *= SimulationSpeed;

    if (deltaTime > 0.1f) {
        Log("Engine", ELogLevel::Warning,
            "Frame took too long (" + std::to_string(deltaTime) + "s). Clamping to avoid physics issues.");
        deltaTime = 0.1f;
    }

    GInputManager->Tick(deltaTime);

    World->Tick(deltaTime);

    Renderer->Tick(deltaTime);
}

void CEngine::Shutdown() {
    Renderer->WaitForIdle();

    World.reset();

    delete GInputManager;
    delete GAssetManager;
    delete Renderer;

    glfwDestroyWindow(Window);
    glfwTerminate();

    delete GLog;
}

void CEngine::CreateWindow() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    Window = glfwCreateWindow(1600, 900, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(Window, this);
}
