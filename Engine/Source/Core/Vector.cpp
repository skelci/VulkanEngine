#include "Vector.hpp"

#include "Rotator.hpp"


SVector2 SVector2::operator+(const SVector2& Other) const { return SVector2(X + Other.X, Y + Other.Y); }

SVector2 SVector2::operator+(float Scalar) const { return SVector2(X + Scalar, Y + Scalar); }

SVector2 SVector2::operator-(const SVector2& Other) const { return SVector2(X - Other.X, Y - Other.Y); }

SVector2 SVector2::operator-(float Scalar) const { return SVector2(X - Scalar, Y - Scalar); }

SVector2 SVector2::operator*(const SVector2& Other) const { return SVector2(X * Other.X, Y * Other.Y); }

SVector2 SVector2::operator*(float Scalar) const { return SVector2(X * Scalar, Y * Scalar); }

SVector2 SVector2::operator/(const SVector2& Other) const { return SVector2(X / Other.X, Y / Other.Y); }

SVector2 SVector2::operator/(float Scalar) const {
    const float InvScalar = 1 / Scalar;
    return SVector2(X * InvScalar, Y * InvScalar);
}

SVector2& SVector2::operator+=(const SVector2& Other) {
    X += Other.X;
    Y += Other.Y;
    return *this;
}

SVector2& SVector2::operator+=(float Scalar) {
    X += Scalar;
    Y += Scalar;
    return *this;
}

SVector2& SVector2::operator-=(const SVector2& Other) {
    X -= Other.X;
    Y -= Other.Y;
    return *this;
}

SVector2& SVector2::operator-=(float Scalar) {
    X -= Scalar;
    Y -= Scalar;
    return *this;
}

SVector2& SVector2::operator*=(const SVector2& Other) {
    X *= Other.X;
    Y *= Other.Y;
    return *this;
}

SVector2& SVector2::operator*=(float Scalar) {
    X *= Scalar;
    Y *= Scalar;
    return *this;
}

SVector2& SVector2::operator/=(const SVector2& Other) {
    X /= Other.X;
    Y /= Other.Y;
    return *this;
}

SVector2& SVector2::operator/=(float Scalar) {
    const float InvScalar = 1 / Scalar;
    X *= InvScalar;
    Y *= InvScalar;
    return *this;
}

float SVector2::LengthSquared() const { return X * X + Y * Y; }

float SVector2::Length() const { return std::sqrt(LengthSquared()); }

SVector2 SVector2::Normalized() const {
    const float InvLen = 1 / Length();
    return SVector2(X * InvLen, Y * InvLen);
}

SVector2 SVector2::SafeNormalized(float Tolerance) const {
    const float Len = Length();
    if (Len < Tolerance) {
        return SVector2();
    }
    const float InvLen = 1.0f / Len;
    return SVector2(X * InvLen, Y * InvLen);
}

SVector2 SVector2::Abs() const { return SVector2(std::abs(X), std::abs(Y)); }

float SVector2::Dot(const SVector2& Other) const { return X * Other.X + Y * Other.Y; }

SVector2 SVector2::Cross(const SVector2& Other) const {
    return SVector2(X * Other.Y - Y * Other.X, Y * Other.X - X * Other.Y);
}

SVector2 SVector2::Rotated(const float Angle) const {
    const float Rad = ToRadians(Angle);
    const float CosA = std::cos(Rad);
    const float SinA = std::sin(Rad);
    return SVector2(X * CosA - Y * SinA, X * SinA + Y * CosA);
}

std::string SVector2::ToString() const { return "SVector2(" + std::to_string(X) + ", " + std::to_string(Y) + ")"; }

// ====== SVector ======

bool SVector::operator==(const SVector& Other) const { return X == Other.X && Y == Other.Y && Z == Other.Z; }

bool SVector::operator!=(const SVector& Other) const { return !(*this == Other); }

SVector SVector::operator+(const SVector& Other) const { return SVector(X + Other.X, Y + Other.Y, Z + Other.Z); }

SVector SVector::operator+(double Scalar) const { return SVector(X + Scalar, Y + Scalar, Z + Scalar); }

SVector SVector::operator-(const SVector& Other) const { return SVector(X - Other.X, Y - Other.Y, Z - Other.Z); }

SVector SVector::operator-(double Scalar) const { return SVector(X - Scalar, Y - Scalar, Z - Scalar); }

SVector SVector::operator-() const { return SVector(-X, -Y, -Z); }

SVector SVector::operator*(const SVector& Other) const { return SVector(X * Other.X, Y * Other.Y, Z * Other.Z); }

SVector SVector::operator*(double Scalar) const { return SVector(X * Scalar, Y * Scalar, Z * Scalar); }

SVector SVector::operator/(const SVector& Other) const { return SVector(X / Other.X, Y / Other.Y, Z / Other.Z); }

SVector SVector::operator/(double Scalar) const {
    const double InvScalar = 1 / Scalar;
    return SVector(X * InvScalar, Y * InvScalar, Z * InvScalar);
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
    const double InvScalar = 1 / Scalar;
    X *= InvScalar;
    Y *= InvScalar;
    Z *= InvScalar;
    return *this;
}

double SVector::LengthSquared() const { return X * X + Y * Y + Z * Z; }

double SVector::Length() const { return std::sqrt(LengthSquared()); }

SVector SVector::Normalized() const {
    const double InvLen = 1 / Length();
    return SVector(X * InvLen, Y * InvLen, Z * InvLen);
}

SVector SVector::SafeNormalized(double Tolerance) const {
    const double Len = Length();
    if (Len < Tolerance) {
        return SVector();
    }
    const double InvLen = 1.0 / Len;
    return SVector(X * InvLen, Y * InvLen, Z * InvLen);
}

SVector SVector::Abs() const { return SVector(std::abs(X), std::abs(Y), std::abs(Z)); }

double SVector::Dot(const SVector& Other) const { return X * Other.X + Y * Other.Y + Z * Other.Z; }

SVector SVector::Cross(const SVector& Other) const {
    return SVector(Y * Other.Z - Z * Other.Y, Z * Other.X - X * Other.Z, X * Other.Y - Y * Other.X);
}

SVector SVector::Rotated(const SRotator& Rotator) const {
    // Convert degrees to radians
    const SRotator RotatorRad = Rotator.AsRadians();

    // Calculate rotation matrices
    const float CP = std::cos(RotatorRad.Pitch);
    const float SP = std::sin(RotatorRad.Pitch);
    const float CY = std::cos(RotatorRad.Yaw);
    const float SY = std::sin(RotatorRad.Yaw);
    const float CR = std::cos(RotatorRad.Roll);
    const float SR = std::sin(RotatorRad.Roll);

    // Z-Up Rotation Matrix (Yaw=Z, Pitch=Y, Roll=X)

    const float M00 = CY * CP;
    const float M01 = -CY * SP * SR - SY * CR;
    const float M02 = -CY * SP * CR + SY * SR;

    const float M10 = SY * CP;
    const float M11 = -SY * SP * SR + CY * CR;
    const float M12 = -SY * SP * CR - CY * SR;

    const float M20 = SP;
    const float M21 = CP * SR;
    const float M22 = CP * CR;

    return SVector(M00 * X + M01 * Y + M02 * Z, M10 * X + M11 * Y + M12 * Z, M20 * X + M21 * Y + M22 * Z);
}

glm::vec3 SVector::ToGLMVec3() const { return glm::vec3(X, Y, Z); }

SRotator SVector::ToRotator() const { return SRotator(X, Y, Z); }

std::string SVector::ToString() const {
    return "SVector(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ")";
}

// ====== SVector4 ======

std::string SVector4::ToString() const {
    return "SVector4(" + std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + ", " +
           std::to_string(A) + ")";
}
