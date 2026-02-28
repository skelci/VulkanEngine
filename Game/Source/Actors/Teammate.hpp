#pragma once

#include "Actors/Character.hpp"
#include "CoreMinimal.hpp"


class ATeammate : public ACharacter {
    CLASS_BODY(ATeammate, ACharacter)

public:
    ATeammate();

    virtual void OnHit(const SHitResult& HitResult, SVector& OutAdjustment) override;
};
