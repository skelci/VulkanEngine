#pragma once

#include "Renderer.hpp"
#include "Vector.hpp"
#include "World.hpp"

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

    CRenderer* Renderer;

    std::unique_ptr<CWorld> World;

public:
    void inline Stop() { IsRunning = false; }

    GLFWwindow* GetWindow() const { return Window; }
    CRenderer* GetRenderer() const { return Renderer; }
    void inline SetActiveCamera(ACamera* Camera) { Renderer->SetActiveCamera(Camera); }

    template <typename T>
    T* AddUIWidget() {
        return Renderer->AddUIWidget<T>();
    }

    CWorld* GetWorld() const { return World.get(); }
};
