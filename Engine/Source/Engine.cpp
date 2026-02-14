#include "Engine.hpp"

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
    ConfigureEngine();

    GLog = new CLog();
    GLog->Initialize();

    glfwInit();
    CreateWindow();

    Renderer = new CRenderer();
    GAssetManager = new CAssetManager();
    GInputManager = new CInputManager();

    Renderer->DefaultTexture = GAssetManager->GetAsset<CTexture>("Engine/Textures/DefaultTexture.png");

    Log("Engine", ELogLevel::Info, "Creating World: " + GEngineConfig.WorldClass->Name);
    World = std::unique_ptr<CWorld>(GEngineConfig.WorldClass.CreateObject());
    World->BeginPlay();

    previousFrameTime = std::chrono::steady_clock::now();
}

void CEngine::MainLoop() {
    glfwPollEvents();
    if (glfwWindowShouldClose(Window)) {
        IsRunning = false;
    }

    const auto now = std::chrono::steady_clock::now();
    const float deltaTime = std::chrono::duration<float>(now - previousFrameTime).count();
    previousFrameTime = now;

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
