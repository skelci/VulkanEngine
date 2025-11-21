#include "Engine.hpp"

#include "Camera.hpp"
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

    std::shared_ptr<SInputMappingContext> EngineMappingContext = std::make_shared<SInputMappingContext>();
    EngineMappingContext->AddMapping(EKeys::Escape, MAKE_INPUT_ACTION(
                                                        EInputValueType::Digital,
                                                        EInputEvent::Pressed,
                                                        this,
                                                        &CEngine::OnEscapePressed));
    EngineMappingContext->AddMapping(EKeys::W, MAKE_INPUT_ACTION(
                                                   EInputValueType::Digital,
                                                   EInputEvent::Triggered,
                                                   this,
                                                   &CEngine::Move, EKeys::W));
    EngineMappingContext->AddMapping(EKeys::S, MAKE_INPUT_ACTION(
                                                   EInputValueType::Digital,
                                                   EInputEvent::Triggered,
                                                   this,
                                                   &CEngine::Move, EKeys::S));
    EngineMappingContext->AddMapping(EKeys::A, MAKE_INPUT_ACTION(
                                                   EInputValueType::Digital,
                                                   EInputEvent::Triggered,
                                                   this,
                                                   &CEngine::Move, EKeys::A));
    EngineMappingContext->AddMapping(EKeys::D, MAKE_INPUT_ACTION(
                                                   EInputValueType::Digital,
                                                   EInputEvent::Triggered,
                                                   this,
                                                   &CEngine::Move, EKeys::D));
    EngineMappingContext->AddMapping(EKeys::Q, MAKE_INPUT_ACTION(
                                                   EInputValueType::Digital,
                                                   EInputEvent::Triggered,
                                                   this,
                                                   &CEngine::Move, EKeys::Q));
    EngineMappingContext->AddMapping(EKeys::E, MAKE_INPUT_ACTION(
                                                   EInputValueType::Digital,
                                                   EInputEvent::Triggered,
                                                   this,
                                                   &CEngine::Move, EKeys::E));
    GInputManager->AddMappingContext(EngineMappingContext);

    Camera = std::make_shared<ACamera>();
    SetActiveCamera(Camera);

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

    Camera->Transform.Position += (InputVector.GetRotated(Camera->Transform.Rotation) + InputHeightVector) * deltaTime;
    Log("Engine", ELogLevel::Info, "Camera Position: " + Camera->Transform.Position.ToString());
    InputVector = SVector(0);
    InputHeightVector = SVector(0);

    Renderer->Tick(deltaTime);
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

void CEngine::Move(EKeys Key) {
    switch (Key) {
    case EKeys::W:
        InputVector.X = 1;
        break;
    case EKeys::S:
        InputVector.X = -1;
        break;
    case EKeys::A:
        InputVector.Y = -1;
        break;
    case EKeys::D:
        InputVector.Y = 1;
        break;
    case EKeys::Q:
        InputHeightVector.Z = -1;
        break;
    case EKeys::E:
        InputHeightVector.Z = 1;
        break;
    }
}
