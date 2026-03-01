#pragma once

#include "Character.hpp"
#include "CoreMinimal.hpp"


class AEnemy : public ACharacter {
    CLASS_BODY(AEnemy, ACharacter)

public:
    AEnemy();

    virtual void Tick(float DeltaTime) override;

    virtual void OnHit(const SHitResult& HitResult, SVector& OutAdjustment) override;

protected:
    bool HasTrash = true;
};
