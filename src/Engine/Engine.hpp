#pragma once

#include "Renderer.hpp"

#include "GLFW/glfw3.h"

#include <memory>

class ACamera;


class CEngine {
public:
    CEngine();

private:
    void Initialize();
    void MainLoop();
    void Shutdown();

    bool IsRunning = true;
    
    GLFWwindow* Window;
    void CreateWindow();

public:
    GLFWwindow* GetWindow() const { return Window; }
    void inline SetActiveCamera(std::shared_ptr<ACamera> Camera) { Renderer->SetActiveCamera(Camera); }
    
private:
    CRenderer* Renderer;

    void OnEscapePressed();
};
