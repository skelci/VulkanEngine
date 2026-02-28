#pragma once

#include "Actors/BoxCollision.hpp"
#include "CoreMinimal.hpp"


class ABoat : public ABoxCollision {
    CLASS_BODY(ABoat, ABoxCollision)

public:
    ABoat();

    virtual void OnHit(const SHitResult& HitResult, SVector& OutAdjustment) override;
};
