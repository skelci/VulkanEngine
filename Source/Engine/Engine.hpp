#pragma once

#include "Renderer.hpp"
#include "Vector.hpp"
#include "World.hpp"

#include "GLFW/glfw3.h"

#include <chrono>
#include <memory>

class ACamera;
class AStaticMeshActor;
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
    GLFWwindow* GetWindow() const { return Window; }
    void inline SetActiveCamera(ACamera* Camera) { Renderer->SetActiveCamera(Camera); }

    CRenderer* GetRenderer() const { return Renderer; }

    CWorld* GetWorld() const { return World.get(); }


    // Game stuff
private:
    ACamera* Camera;
    void OnEscapePressed();

    void Move(EKeys Key);
    SVector InputVector = SVector(0);

    void LookAround(SVector2 Delta);
    float FlySpeed = 1;
    void ScaleFlySpeed(float Amount);

    void InitGameStuff();

    AStaticMeshActor* VikingRoomActor;
    void TickGameStuff(float DeltaTime);
};
