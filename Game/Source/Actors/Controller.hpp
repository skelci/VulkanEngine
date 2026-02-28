#pragma once

#include "Actors/Actor.hpp"
#include "CoreMinimal.hpp"


class AController : public AActor {
    CLASS_BODY(AController, AActor)

public:
    AController();

    virtual void Tick(float DeltaTime) override;

    class ACamera* Camera;
    class AActor* CameraArm;

    void Move(EKeys Key);
    void Look(SVector2 Delta);
    void UpdateCameraDistance(float Amount);

    SVector InputVector;
    float MovementSpeed = 6.0f;
    float MinCameraDistance = 2.0f;
    float MaxCameraDistance = 10.0f;
    float DesiredRotation = 0.0f;

    // If false, the parent rotation will be snapped to controller rotation.
    bool RotateTowardsMovement = false;
    bool DisableSteering = false;
    float RotationSpeed = 180.0f;
};
