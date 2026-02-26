#pragma once

#include "Actors/BoxCollision.hpp"
#include "CoreMinimal.hpp"


class ACharacter : public ABoxCollision {
    CLASS_BODY(ACharacter, ABoxCollision)

public:
    ACharacter();

    virtual void Tick(float DeltaTime) override;

protected:
    class AMeshActor* Mesh;
    class ACamera* Camera;

    void Move(EKeys Key);
    void Look(SVector2 Delta);
    void UpdateCameraDistance(float Amount);

    SVector InputVector;
    float CameraDistance = 5.0f;
    float MinCameraDistance = 1.0f;
    float MaxCameraDistance = 10.0f;

    void MoveCamera();
};
