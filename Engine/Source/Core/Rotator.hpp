#pragma once

#include <string>

struct SVector;


struct SRotator {
    float Pitch;
    float Yaw;
    float Roll;

    SRotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
    SRotator(float uniform) : Pitch(uniform), Yaw(uniform), Roll(uniform) {}
    SRotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}

    bool operator==(const SRotator& Other) const;
    bool operator!=(const SRotator& Other) const;

    SRotator operator+(const SRotator& Other) const;
    SRotator& operator+=(const SRotator& Other);

    SRotator InterpTo(const SRotator& Target, float DeltaTime, float InterpSpeed) const;

    SVector ForwardVector() const;

    SRotator AsRadians() const;
    SVector ToEuler() const;
    std::string ToString() const;
};
