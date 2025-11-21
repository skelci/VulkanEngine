#include "Vector.hpp"

#include "Math.hpp"
#include "Rotator.hpp"


SVector SVector::operator+(const SVector& Other) const {
    return SVector(X + Other.X, Y + Other.Y, Z + Other.Z);
}

SVector SVector::operator+(double Scalar) const {
    return SVector(X + Scalar, Y + Scalar, Z + Scalar);
}

SVector SVector::operator-(const SVector& Other) const {
    return SVector(X - Other.X, Y - Other.Y, Z - Other.Z);
}

SVector SVector::operator-(double Scalar) const {
    return SVector(X - Scalar, Y - Scalar, Z - Scalar);
}

SVector SVector::operator*(const SVector& Other) const {
    return SVector(X * Other.X, Y * Other.Y, Z * Other.Z);
}

SVector SVector::operator*(double Scalar) const {
    return SVector(X * Scalar, Y * Scalar, Z * Scalar);
}

SVector SVector::operator/(const SVector& Other) const {
    return SVector(X / Other.X, Y / Other.Y, Z / Other.Z);
}

SVector SVector::operator/(double Scalar) const {
    return SVector(X / Scalar, Y / Scalar, Z / Scalar);
}

SVector& SVector::operator+=(const SVector& Other) {
    X += Other.X;
    Y += Other.Y;
    Z += Other.Z;
    return *this;
}

SVector& SVector::operator+=(double Scalar) {
    X += Scalar;
    Y += Scalar;
    Z += Scalar;
    return *this;
}

SVector& SVector::operator-=(const SVector& Other) {
    X -= Other.X;
    Y -= Other.Y;
    Z -= Other.Z;
    return *this;
}

SVector& SVector::operator-=(double Scalar) {
    X -= Scalar;
    Y -= Scalar;
    Z -= Scalar;
    return *this;
}

SVector& SVector::operator*=(const SVector& Other) {
    X *= Other.X;
    Y *= Other.Y;
    Z *= Other.Z;
    return *this;
}

SVector& SVector::operator*=(double Scalar) {
    X *= Scalar;
    Y *= Scalar;
    Z *= Scalar;
    return *this;
}

SVector& SVector::operator/=(const SVector& Other) {
    X /= Other.X;
    Y /= Other.Y;
    Z /= Other.Z;
    return *this;
}

SVector& SVector::operator/=(double Scalar) {
    X /= Scalar;
    Y /= Scalar;
    Z /= Scalar;
    return *this;
}

double SVector::LengthSquared() const {
    return X * X + Y * Y + Z * Z;
}

double SVector::Length() const {
    return std::sqrt(LengthSquared());
}

SVector SVector::GetNormalized() const {
    return SVector(X / Length(), Y / Length(), Z / Length());
}

SVector SVector::GetSafeNormalized(double Tolerance) const {
    const double Len = Length();
    if (Len > Tolerance) {
        return SVector(X / Len, Y / Len, Z / Len);
    }
    return SVector();
}

SVector SVector::GetAbs() const {
    return SVector(std::abs(X), std::abs(Y), std::abs(Z));
}

double SVector::Dot(const SVector& Other) const {
    return X * Other.X + Y * Other.Y + Z * Other.Z;
}

SVector SVector::Cross(const SVector& Other) const {
    return SVector(
        Y * Other.Z - Z * Other.Y,
        Z * Other.X - X * Other.Z,
        X * Other.Y - Y * Other.X);
}

SVector SVector::GetRotated(const SRotator& Rotator) const {
    // Convert degrees to radians
    const SRotator RotatorRad = Rotator.AsRadians();

    // Calculate rotation matrices
    const float CP = std::cos(RotatorRad.Pitch);
    const float SP = std::sin(RotatorRad.Pitch);
    const float CY = std::cos(RotatorRad.Yaw);
    const float SY = std::sin(RotatorRad.Yaw);
    const float CR = std::cos(RotatorRad.Roll);
    const float SR = std::sin(RotatorRad.Roll);

    // Combined rotation matrix
    const float M00 = CY * CR;
    const float M01 = CY * SR * SP - SY * CP;
    const float M02 = CY * SR * CP + SY * SP;

    const float M10 = SY * CR;
    const float M11 = SY * SR * SP + CY * CP;
    const float M12 = SY * SR * CP - CY * SP;

    const float M20 = -SR;
    const float M21 = CR * SP;
    const float M22 = CR * CP;

    // Apply rotation
    return SVector(
        M00 * X + M01 * Y + M02 * Z,
        M10 * X + M11 * Y + M12 * Z,
        M20 * X + M21 * Y + M22 * Z);
}

glm::vec3 SVector::ToGLMVec3() const {
    return glm::vec3(X, Y, Z);
}

std::string SVector::ToString() const {
    return "SVector(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ")";
}
