#pragma once

#include "Renderer.hpp"
#include "Vector.hpp"
#include "World.hpp"

#include "GLFW/glfw3.h"

#include <chrono>
#include <memory>


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

    TSubclassOf<CWorld> PendingWorldClass;
    std::unique_ptr<CWorld> World;

public:
    void Stop() { IsRunning = false; }

    GLFWwindow* GetWindow() const { return Window; }
    CRenderer* GetRenderer() const { return Renderer; }

    CWorld* GetWorld() const { return World.get(); }
    void OpenWorld(const TSubclassOf<CWorld>& WorldClass) { PendingWorldClass = WorldClass; }
};
