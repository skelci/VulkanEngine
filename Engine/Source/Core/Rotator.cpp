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

SRotator SRotator::InterpTo(const SRotator& Target, float DeltaTime, float InterpSpeed) const {
    const float InterpFactor = 1 - std::exp(-InterpSpeed * DeltaTime);
    return SRotator(
        Pitch + (Target.Pitch - Pitch) * InterpFactor, Yaw + (Target.Yaw - Yaw) * InterpFactor,
        Roll + (Target.Roll - Roll) * InterpFactor
    );
}

SVector SRotator::ForwardVector() const {
    const float P = ToRadians(Pitch);
    const float Y = ToRadians(Yaw);
    const float CP = std::cos(P);
    const float SP = std::sin(P);
    const float CY = std::cos(Y);
    const float SY = std::sin(Y);

    return SVector(CP * CY, CP * SY, SP);
}

SRotator SRotator::AsRadians() const { return SRotator(ToRadians(Pitch), ToRadians(Yaw), ToRadians(Roll)); }

SVector SRotator::ToEuler() const { return SVector(Roll, Pitch, Yaw); }

std::string SRotator::ToString() const {
    return "SRotator(" + std::to_string(Pitch) + ", " + std::to_string(Yaw) + ", " + std::to_string(Roll) + ")";
}
