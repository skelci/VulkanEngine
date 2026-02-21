#include "GameMap.hpp"

#include "VikingRoom.hpp"

#include "Actors/Camera.hpp"
#include "Assets/Material.hpp"
#include "Assets/Model.hpp"


void CGameMap::BeginPlay() {
    GInputManager->SetInputMode(false);

    SInputMappingContext* GameMC = new SInputMappingContext();
    GameMC->AddMapping(
        EKeys::Escape,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Pressed, this, &CGameMap::OnEscapePressed)
    );
    GameMC->AddMapping(
        EKeys::W, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CGameMap::Move, EKeys::W)
    );
    GameMC->AddMapping(
        EKeys::S, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CGameMap::Move, EKeys::S)
    );
    GameMC->AddMapping(
        EKeys::A, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CGameMap::Move, EKeys::A)
    );
    GameMC->AddMapping(
        EKeys::D, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CGameMap::Move, EKeys::D)
    );
    GameMC->AddMapping(
        EKeys::Q, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CGameMap::Move, EKeys::Q)
    );
    GameMC->AddMapping(
        EKeys::E, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &CGameMap::Move, EKeys::E)
    );
    GameMC->AddMapping(
        EKeys::Mouse2D, MAKE_INPUT_ACTION(EInputValueType::Axis2D, EInputEvent::Triggered, this, &CGameMap::LookAround)
    );
    GameMC->AddMapping(
        EKeys::MouseWheelAxis,
        MAKE_INPUT_ACTION(EInputValueType::Axis1D, EInputEvent::Triggered, this, &CGameMap::ScaleFlySpeed)
    );
    GInputManager->AddMappingContext(GameMC);

    Camera = SpawnActor<ACamera>();
    Camera->Transform.Position = SVector(0, 0, 5);
    GEngine->GetRenderer()->SetActiveCamera(Camera);

    for (int32 i = 0; i < 100; i++) {
        AVikingRoom* actor = SpawnActor<AVikingRoom>();
        actor->Transform.Position = SVector((i % 10) * 3.0, (i / 10) * 3.0, 0);
    }

    Character = SpawnActor<AMeshActor>();
    Character->Model = GetAsset<CModel>("Game/Character/character.obj");
    std::shared_ptr<CMaterial> mat = GetAsset<CMaterial>("Engine/Materials/SimpleShading.mat");
    mat->SetProperty("LightColor", SVector(1.5));
    Character->Model->SetMaterial(mat);
    Character->Transform.Position = SVector(-3, 0, 0);
    Character->Transform.Rotation = SRotator(0, 0, 90);
}

void CGameMap::Tick(float DeltaTime) {
    Camera->Transform.Position +=
        (SVector(InputVector.X, InputVector.Y, 0).Rotated(Camera->Transform.Rotation) + SVector(0, 0, InputVector.Z)) *
        FlySpeed * DeltaTime;

    InputVector = SVector(0);

    Character->Transform.Rotation.Yaw += 30 * DeltaTime;
}

void CGameMap::OnEscapePressed() { GEngine->Stop(); }

void CGameMap::Move(EKeys Key) {
    switch (Key) {
    case EKeys::W: InputVector.X += 1; break;
    case EKeys::S: InputVector.X += -1; break;
    case EKeys::A: InputVector.Y += -1; break;
    case EKeys::D: InputVector.Y += 1; break;
    case EKeys::Q: InputVector.Z += -1; break;
    case EKeys::E: InputVector.Z += 1; break;
    }
}

void CGameMap::LookAround(SVector2 Delta) {
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

void CGameMap::ScaleFlySpeed(float Amount) { FlySpeed *= 1 + Amount * 0.1f; }
