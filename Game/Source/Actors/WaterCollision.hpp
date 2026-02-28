#pragma once

#include "Actors/BoxCollision.hpp"
#include "CoreMinimal.hpp"


class AWaterCollision : public ABoxCollision {
    CLASS_BODY(AWaterCollision, ABoxCollision)

public:
    AWaterCollision();
};