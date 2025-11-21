#pragma once

#include "Renderer.hpp"
#include "Vector.hpp"

#include "GLFW/glfw3.h"

#include <chrono>
#include <memory>

class ACamera;
enum class EKeys : uint16_t;


class CEngine {
public:
    CEngine();

private:
    void Initialize();
    void MainLoop();
    void Shutdown();

    bool IsRunning = true;
    std::chrono::steady_clock::time_point previousFrameTime;

    GLFWwindow* Window;
    void CreateWindow();

public:
    GLFWwindow* GetWindow() const { return Window; }
    void inline SetActiveCamera(std::shared_ptr<ACamera> Camera) { Renderer->SetActiveCamera(Camera); }

private:
    CRenderer* Renderer;

    void OnEscapePressed();

public:
    std::shared_ptr<ACamera> Camera;

    void Move(EKeys Key);
    SVector InputVector = SVector(0);
    SVector InputHeightVector = SVector(0);
};
