#include "Character.hpp"

#include "Actors/Camera.hpp"
#include "Actors/MeshActor.hpp"


ACharacter::ACharacter() {
    SimulatePhysics = true;
    SetBoxExtent(SVector(0.25f, 0.25f, 0.6f));
    SetVisibility(true);

    Mesh = SpawnChildActor<AMeshActor>(AMeshActor::StaticClass());
    Mesh->Model = GetAsset<CModel>("Game/Character/burnice.fbx");
    Mesh->Model->SetMaterialProperty("LightColor", SVector3(1.5f, 1.5f, 1.5f));
    Mesh->Model->SetMaterialProperty("AmbientLight", SVector3(0.5f));
    Mesh->Transform.Rotation.Yaw = 90.0f;
    Mesh->Transform.Position.Z = -0.57f;

    Camera = SpawnChildActor<ACamera>(ACamera::StaticClass());
    GEngine->GetRenderer()->SetActiveCamera(Camera);
    MoveCamera();

    SInputMappingContext* MC = new SInputMappingContext();
    MC->AddMapping(
        EKeys::W, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &ACharacter::Move, EKeys::W)
    );
    MC->AddMapping(
        EKeys::S, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &ACharacter::Move, EKeys::S)
    );
    MC->AddMapping(
        EKeys::A, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &ACharacter::Move, EKeys::A)
    );
    MC->AddMapping(
        EKeys::D, MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &ACharacter::Move, EKeys::D)
    );
    MC->AddMapping(
        EKeys::Mouse2D, MAKE_INPUT_ACTION(EInputValueType::Axis2D, EInputEvent::Triggered, this, &ACharacter::Look)
    );
    MC->AddMapping(
        EKeys::MouseWheelAxis,
        MAKE_INPUT_ACTION(EInputValueType::Axis1D, EInputEvent::Triggered, this, &ACharacter::UpdateCameraDistance)
    );
    GInputManager->AddMappingContext(MC);
}

void ACharacter::Tick(float DeltaTime) {
    SVector NewVelocity = InputVector.SafeNormalized().Rotated(Transform.Rotation) * 6.0f;
    NewVelocity.Z = Velocity.Z;
    Velocity = Velocity * 0.8f + NewVelocity * 0.2f;
    InputVector = SVector(0);

    Super::Tick(DeltaTime);
}

void ACharacter::Move(EKeys Key) {
    switch (Key) {
    case EKeys::W: InputVector.X += 1; break;
    case EKeys::S: InputVector.X += -1; break;
    case EKeys::A: InputVector.Y += -1; break;
    case EKeys::D: InputVector.Y += 1; break;
    }
}

void ACharacter::Look(SVector2 Delta) {
    const SVector2 ScaledDelta = Delta * 0.1f;
    SRotator& CameraRotation = Camera->Transform.Rotation;
    Transform.Rotation.Yaw += ScaledDelta.X;
    CameraRotation.Pitch += -ScaledDelta.Y;
    if (CameraRotation.Pitch > 89.9f) {
        CameraRotation.Pitch = 89.9f;
    }
    if (CameraRotation.Pitch < -89.9f) {
        CameraRotation.Pitch = -89.9f;
    }
    MoveCamera();
}

void ACharacter::UpdateCameraDistance(float Amount) {
    CameraDistance += -Amount * 0.3f;
    CameraDistance = std::clamp(CameraDistance, MinCameraDistance, MaxCameraDistance);
    MoveCamera();
}

void ACharacter::MoveCamera() {
    Camera->Transform.Position = SVector(-CameraDistance, 0, 0).Rotated(Camera->Transform.Rotation);
}
