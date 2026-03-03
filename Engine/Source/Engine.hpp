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
    bool IsWorldTransitioning = false;

    CObject* GameInstance;

public:
    void Stop() { IsRunning = false; }

    GLFWwindow* GetWindow() const { return Window; }
    CRenderer* GetRenderer() const { return Renderer; }

    CWorld* GetWorld() const;
    void OpenWorld(const TSubclassOf<CWorld>& WorldClass) { PendingWorldClass = WorldClass; }

    template <typename T>
    T* GetGameInstance() const {
        return Cast<T>(GameInstance);
    }

    float SimulationSpeed = 1.0f;
};
