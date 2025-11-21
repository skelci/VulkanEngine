#pragma once

#include "CoreMinimal.hpp"

struct STransform {
    glm::vec3 Position = glm::vec3(0);
    SRotator Rotation = SRotator();
    glm::vec3 Scale = glm::vec3(1);
};


class AActor {
public:
    STransform Transform;
};
