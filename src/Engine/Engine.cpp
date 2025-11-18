#include "Engine.hpp"

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
    GLog = new CLog();
    GLog->Initialize();

    GInputManager = new CInputManager();

    glfwInit();
    CreateWindow();

    Renderer = new CRenderer();

    SInputMappingContext* EngineMappingContext = new SInputMappingContext();
    EngineMappingContext->AddMapping(EKeys::Escape, INPUT_ACTION(
        EInputValueType::Digital,
        EInputEvent::Pressed,
        this,
        &CEngine::OnEscapePressed
    ));

    GInputManager->AddMappingContext(std::shared_ptr<SInputMappingContext>(EngineMappingContext));
}

void CEngine::MainLoop() {
    glfwPollEvents();
    if (glfwWindowShouldClose(Window)) {
        IsRunning = false;
    }

    GInputManager->Tick(0.0f);
    Renderer->Tick(0.0f);
}

void CEngine::Shutdown() {
    delete Renderer;
    delete GInputManager;
    delete GLog;
}

void CEngine::CreateWindow() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    Window = glfwCreateWindow(1600, 900, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(Window, this);
}

void CEngine::OnEscapePressed() {
    IsRunning = false;
}
