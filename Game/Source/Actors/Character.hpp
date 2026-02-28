#pragma once

#include "Actors/BoxCollision.hpp"
#include "CoreMinimal.hpp"


class ACharacter : public ABoxCollision {
    CLASS_BODY(ACharacter, ABoxCollision)

public:
    ACharacter();

    virtual void OnHit(const SHitResult& HitResult, SVector& OutAdjustment) override;

protected:
    class AMeshActor* Mesh;
};
