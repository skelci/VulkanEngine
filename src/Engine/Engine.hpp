#pragma once

#include "Renderer.hpp"


class CEngine {
public:
    CEngine();

private:
    void Initialize();
    void MainLoop();
    void Shutdown();

    bool IsRunning = true;

    CRenderer* Renderer;

    void OnWindowClosed();
};
