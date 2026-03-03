#pragma once

#include "Actors/BoxCollision.hpp"
#include "CoreMinimal.hpp"


class ATrash : public ABoxCollision {
    CLASS_BODY(ATrash, ABoxCollision)

public:
    ATrash();
    ~ATrash();

    virtual void Tick(float DeltaTime) override;
};
