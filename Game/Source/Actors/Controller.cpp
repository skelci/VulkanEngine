#include "Controller.hpp"

#include "Actors/Camera.hpp"
#include "Actors/PhysicsBody.hpp"


AController::AController() {
    CameraArm = SpawnChildActor<AActor>(AActor::StaticClass());
    Camera = CameraArm->SpawnChildActor<ACamera>(ACamera::StaticClass());
    Camera->Transform.Position.X = -5.0f;
    GEngine->GetRenderer()->SetActiveCamera(Camera);

    SInputMappingContext* MC = new SInputMappingContext();
    MC->AddMapping(
        EKeys::W,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &AController::Move, EKeys::W)
    );
    MC->AddMapping(
        EKeys::S,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &AController::Move, EKeys::S)
    );
    MC->AddMapping(
        EKeys::A,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &AController::Move, EKeys::A)
    );
    MC->AddMapping(
        EKeys::D,
        MAKE_INPUT_ACTION(EInputValueType::Digital, EInputEvent::Triggered, this, &AController::Move, EKeys::D)
    );
    MC->AddMapping(
        EKeys::Mouse2D, MAKE_INPUT_ACTION(EInputValueType::Axis2D, EInputEvent::Triggered, this, &AController::Look)
    );
    MC->AddMapping(
        EKeys::MouseWheelAxis,
        MAKE_INPUT_ACTION(EInputValueType::Axis1D, EInputEvent::Triggered, this, &AController::UpdateCameraDistance)
    );
    GInputManager->AddMappingContext(MC);
}

void AController::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    const STransform ParentTransform = Parent->GetWorldTransform();
    const SVector NormalizedInput = InputVector.SafeNormalizedXY();

    SVector& Velocity = Cast<APhysicsBody>(Parent)->Velocity;
    const SRotator MovementForward = DisableSteering ? ParentTransform.Rotation : SRotator(0, DesiredRotation, 0);
    SVector NewVelocity =
        (DisableSteering ? SVector(InputVector.X, 0, 0) : NormalizedInput).Rotated(MovementForward) * MovementSpeed;
    NewVelocity.Z = Velocity.Z;
    Velocity = Velocity * 0.8f + NewVelocity * 0.2f;

    if (RotateTowardsMovement && !DisableSteering && Velocity.LengthXYSquared() > SMALL_NUMBER) {
        const SVector ViewDir = Velocity.NormalizedXY();
        const float NewAngle = ToDegrees(std::atan2(ViewDir.Y, ViewDir.X));

        float& ParentRotation = Parent->Transform.Rotation.Yaw;
        ParentRotation = RInterpTo(ParentRotation, NewAngle, DeltaTime, RotationSpeed);
    }

    if (DisableSteering) {
        float& ParentRotation = Parent->Transform.Rotation.Yaw;
        ParentRotation +=
            RotationSpeed * InputVector.Y * InputVector.X * Velocity.LengthXY() / MovementSpeed * DeltaTime;
    }

    InputVector = SVector(0);
    Transform.Rotation.Yaw = DesiredRotation - Parent->Transform.Rotation.Yaw;
}

void AController::Move(EKeys Key) {
    switch (Key) {
    case EKeys::W: InputVector.X += 1; break;
    case EKeys::S: InputVector.X += -1; break;
    case EKeys::A: InputVector.Y += -1; break;
    case EKeys::D: InputVector.Y += 1; break;
    }
}

void AController::Look(SVector2 Delta) {
    const SVector2 ScaledDelta = Delta * 0.1f;
    SRotator& ArmRotation = CameraArm->Transform.Rotation;
    ArmRotation.Pitch += -ScaledDelta.Y;
    ArmRotation.Pitch = std::clamp(ArmRotation.Pitch, -89.0f, 89.0f);
    float& ParentRotation = Parent->Transform.Rotation.Yaw;

    DesiredRotation += ScaledDelta.X;
    if (!RotateTowardsMovement) {
        ParentRotation += ScaledDelta.X;
    }
}

void AController::UpdateCameraDistance(float Amount) {
    double& CameraDistance = Camera->Transform.Position.X;
    CameraDistance += Amount * 0.3f;
    CameraDistance = std::clamp((float)CameraDistance, -MaxCameraDistance, -MinCameraDistance);
}
