#include "Rotator.hpp"

#include "Math.hpp"
#include "Vector.hpp"


SRotator SRotator::operator+(const SRotator& Other) const {
    return SRotator(Pitch + Other.Pitch, Yaw + Other.Yaw, Roll + Other.Roll);
}

SRotator& SRotator::operator+=(const SRotator& Other) {
    Pitch += Other.Pitch;
    Yaw += Other.Yaw;
    Roll += Other.Roll;
    return *this;
}

SRotator SRotator::AsRadians() const { return SRotator(ToRadians(Pitch), ToRadians(Yaw), ToRadians(Roll)); }

SVector SRotator::ToVector() const { return SVector(Pitch, Yaw, Roll); }

SVector SRotator::GetForwardVector() const {
    float CP = std::cos(ToRadians(Pitch));
    float SP = std::sin(ToRadians(Pitch));
    float CY = std::cos(ToRadians(Yaw));
    float SY = std::sin(ToRadians(Yaw));

    return SVector(CP * CY, CP * SY, SP);
}

std::string SRotator::ToString() const {
    return "SRotator(" + std::to_string(Pitch) + ", " + std::to_string(Yaw) + ", " + std::to_string(Roll) + ")";
}
