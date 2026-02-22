#include "GameMap.hpp"

#include "Actors/Camera.hpp"
#include "Actors/MeshActor.hpp"
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

    AMeshActor* Water = SpawnActor<AMeshActor>();
    Water->Model = GetAsset<CModel>("Game/Meshes/HighResPlane.fbx");
    Water->Model->SetMaterial(GetAsset<CMaterial>("Game/Materials/Water.mat"));
    Water->Transform.Scale = SVector(1000, 1000, 1);

    AMeshActor* Sand = SpawnActor<AMeshActor>();
    Sand->Model = GetAsset<CModel>("Game/Meshes/Island.fbx");
    Sand->Model->SetMaterial(GetAsset<CMaterial>("Game/Materials/Sand.mat"));
    Sand->Transform.Scale = SVector(100, 100, 100);
    Sand->Transform.Position = SVector(0.0f, 0.0f, -1.5f);

    Character = SpawnActor<AMeshActor>();
    Character->Model = GetAsset<CModel>("Game/Character/burnice.fbx");
    Character->Model->SetMaterialProperty("LightColor", SVector3(1.5f, 1.5f, 1.5f));
    Character->Model->SetMaterialProperty("AmbientLight", SVector3(0.5f));
    Character->Transform.Position = SVector(0, 0, 10);
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
