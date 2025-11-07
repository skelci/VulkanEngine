#include "Engine.hpp"
#include "Log.hpp"


CEngine::CEngine() {
    Initialize();
    MainLoop();
    Shutdown();
}

void CEngine::Initialize() {
    CLog::Init();

    Renderer = new CRenderer();
    Renderer->OnWindowClosed.Bind(this, &CEngine::OnWindowClosed);
}

void CEngine::MainLoop() {
    while (IsRunning) {
        Renderer->Tick(0.0f);
    }
}

void CEngine::Shutdown() {
    delete Renderer;
}

void CEngine::OnWindowClosed() {
    IsRunning = false;
}
