#pragma once

#include "CoreMinimal.hpp"

struct STransform {
    SVector Position = SVector(0);
    SRotator Rotation = SRotator();
    SVector Scale = SVector(1);
};


class AActor {
public:
    STransform Transform;
};
