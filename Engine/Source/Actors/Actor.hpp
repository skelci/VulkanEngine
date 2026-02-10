#pragma once

#include "Object.hpp"
#include "Rotator.hpp"
#include "Vector.hpp"


struct STransform {
    SVector Position = SVector(0);
    SRotator Rotation = SRotator(0);
    SVector Scale = SVector(1);
};


class AActor : public CObject {
    CLASS_BODY(AActor, CObject)
public:
    virtual ~AActor();
    STransform Transform;
};
