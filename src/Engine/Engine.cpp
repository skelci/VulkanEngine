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

    glfwInit();
    CreateWindow();

    GInputManager = new CInputManager();

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
    EngineMappingContext->AddMapping(EKeys::Mouse2D, MAKE_INPUT_ACTION(
                                                         EInputValueType::Axis2D,
                                                         EInputEvent::Triggered,
                                                         this,
                                                         &CEngine::LookAround));
    EngineMappingContext->AddMapping(EKeys::MouseWheelAxis, MAKE_INPUT_ACTION(
                                                                EInputValueType::Axis1D,
                                                                EInputEvent::Triggered,
                                                                this,
                                                                &CEngine::ScaleFlySpeed));
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

    Camera->Transform.Position += (SVector(InputVector.X, InputVector.Y, 0).GetRotated(Camera->Transform.Rotation) + SVector(0, 0, InputVector.Z)) * FlySpeed * deltaTime;

    InputVector = SVector(0);

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
        InputVector.Z = -1;
        break;
    case EKeys::E:
        InputVector.Z = 1;
        break;
    }
}

void CEngine::LookAround(SVector2 Delta) {
    SRotator& CameraRotation = Camera->Transform.Rotation;
    const SVector2 ScaledDelta = Delta * 0.1;
    CameraRotation += SRotator(-ScaledDelta.Y, ScaledDelta.X, 0);
    if (CameraRotation.Pitch > 89.9f) {
        CameraRotation.Pitch = 89.9f;
    }
    if (CameraRotation.Pitch < -89.9f) {
        CameraRotation.Pitch = -89.9f;
    }
}

void CEngine::ScaleFlySpeed(float Amount) {
    FlySpeed *= 1 + Amount * 0.1f;
}
