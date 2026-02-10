#pragma once

#include "CoreMinimal.hpp"

#include "Actor.hpp"

class ACamera : public AActor {
public:
    float FOV = 90.0f;
    float NearClip = 0.1f;
    float FarClip = 1000.0f;
};
