#pragma once

#include <glm/glm.hpp>

struct SRotator {
    float Pitch;
    float Yaw;
    float Roll;

    SRotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
    SRotator(float uniform) : Pitch(uniform), Yaw(uniform), Roll(uniform) {}
    SRotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}

    glm::vec3 GetVector() const { return glm::vec3(Pitch, Yaw, Roll); }
    glm::vec3 GetForwardVector() const;
};
