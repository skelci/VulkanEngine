#pragma once

#include "Renderer.hpp"

#include "GLFW/glfw3.h"


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
    
private:
    CRenderer* Renderer;

    void OnEscapePressed();
};
