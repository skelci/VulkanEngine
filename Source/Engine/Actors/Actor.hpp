#pragma once

#include "Rotator.hpp"
#include "Vector.hpp"


struct STransform {
    SVector Position = SVector(0);
    SRotator Rotation = SRotator(0);
    SVector Scale = SVector(1);
};


class AActor {
public:
    virtual ~AActor();
    STransform Transform;
};
