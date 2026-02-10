#include "TestWorld.hpp"

#include "Actors/Camera.hpp"
#include "Actors/StaticMeshActor.hpp"


void CTestWorld::BeginPlay() {
    SInputMappingContext* GameMC = new SInputMappingContext();
    GameMC->AddMapping(
        EKeys::Escape,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Pressed, this, &CTestWorld::OnEscapePressed)
    );
    GameMC->AddMapping(
        EKeys::W, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CTestWorld::Move, EKeys::W)
    );
    GameMC->AddMapping(
        EKeys::S, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CTestWorld::Move, EKeys::S)
    );
    GameMC->AddMapping(
        EKeys::A, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CTestWorld::Move, EKeys::A)
    );
    GameMC->AddMapping(
        EKeys::D, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CTestWorld::Move, EKeys::D)
    );
    GameMC->AddMapping(
        EKeys::Q, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CTestWorld::Move, EKeys::Q)
    );
    GameMC->AddMapping(
        EKeys::E, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CTestWorld::Move, EKeys::E)
    );
    GameMC->AddMapping(
        EKeys::Mouse2D,
        MAKE_INPUT_ACTION(EInputValueType::Axis2D, EInputEvent::Triggered, this, &CTestWorld::LookAround)
    );
    GameMC->AddMapping(
        EKeys::MouseWheelAxis,
        MAKE_INPUT_ACTION(EInputValueType::Axis1D, EInputEvent::Triggered, this, &CTestWorld::ScaleFlySpeed)
    );
    GInputManager->AddMappingContext(GameMC);

    Camera = GEngine->GetWorld()->SpawnActor<ACamera>();
    GEngine->SetActiveCamera(Camera);

    auto vikingRoomTexture = GetAsset<CTexture>("Game/viking_room/viking_room.png");

    VikingRoomActor = GEngine->GetWorld()->SpawnActor<AStaticMeshActor>();
    VikingRoomActor->StaticMesh = GetAsset<CStaticMesh>("Game/viking_room/viking_room.obj");
    VikingRoomActor->Texture = vikingRoomTexture;

    for (int i = 0; i < 100; i++) {
        AStaticMeshActor* actor = GEngine->GetWorld()->SpawnActor<AStaticMeshActor>();
        actor->StaticMesh = GetAsset<CStaticMesh>("Game/viking_room/viking_room.obj");
        actor->Texture = vikingRoomTexture;
        actor->Transform.Position = SVector((i % 10) * 3.0, (i / 10) * 3.0, 0);
    }

    auto character = GEngine->GetWorld()->SpawnActor<AStaticMeshActor>();
    character->StaticMesh = GetAsset<CStaticMesh>("Game/Character/character.obj");
    character->Texture = GetAsset<CTexture>("Game/textures/texture.jpeg");
    character->Transform.Scale = SVector(5.0);
    character->Transform.Position = SVector(-5.0);
}

void CTestWorld::Tick(float DeltaTime) {
    Camera->Transform.Position +=
        (SVector(InputVector.X, InputVector.Y, 0).Rotated(Camera->Transform.Rotation) + SVector(0, 0, InputVector.Z)) *
        FlySpeed * DeltaTime;

    InputVector = SVector(0);

    VikingRoomActor->Transform.Rotation.Yaw += 10.0f * DeltaTime;
    VikingRoomActor->Transform.Position.Z += -0.1 * DeltaTime;
    VikingRoomActor->Transform.Scale += 0.1 * DeltaTime;

    Log("Game", ELogLevel::Info, "FPS: " + std::to_string(1.0f / DeltaTime));
}

void CTestWorld::OnEscapePressed() { GEngine->Stop(); }

void CTestWorld::Move(EKeys Key) {
    switch (Key) {
    case EKeys::W: InputVector.X += 1; break;
    case EKeys::S: InputVector.X += -1; break;
    case EKeys::A: InputVector.Y += -1; break;
    case EKeys::D: InputVector.Y += 1; break;
    case EKeys::Q: InputVector.Z += -1; break;
    case EKeys::E: InputVector.Z += 1; break;
    }
}

void CTestWorld::LookAround(SVector2 Delta) {
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

void CTestWorld::ScaleFlySpeed(float Amount) { FlySpeed *= 1 + Amount * 0.1f; }
