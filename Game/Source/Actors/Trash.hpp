#pragma once

#include "Actors/BoxCollision.hpp"
#include "CoreMinimal.hpp"


class ATrash : public ABoxCollision {
    CLASS_BODY(ATrash, ABoxCollision)

public:
    ATrash();
};
