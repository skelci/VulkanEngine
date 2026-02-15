#pragma once

#include "Math.hpp"

#include <glm/glm.hpp>

#include <cmath>
#include <string>

struct SRotator;


struct SVector2 {
    double X;
    double Y;

    SVector2() : X(0), Y(0) {}
    SVector2(double uniform) : X(uniform), Y(uniform) {}
    SVector2(double x, double y) : X(x), Y(y) {}

    SVector2 operator+(const SVector2& Other) const;
    SVector2 operator+(double Scalar) const;
    SVector2 operator-(const SVector2& Other) const;
    SVector2 operator-(double Scalar) const;

    SVector2 operator*(const SVector2& Other) const;
    SVector2 operator*(double Scalar) const;
    SVector2 operator/(const SVector2& Other) const;
    SVector2 operator/(double Scalar) const;

    SVector2& operator+=(const SVector2& Other);
    SVector2& operator+=(double Scalar);
    SVector2& operator-=(const SVector2& Other);
    SVector2& operator-=(double Scalar);

    SVector2& operator*=(const SVector2& Other);
    SVector2& operator*=(double Scalar);
    SVector2& operator/=(const SVector2& Other);
    SVector2& operator/=(double Scalar);

    double LengthSquared() const;
    double Length() const;

    SVector2 Normalized() const;
    SVector2 SafeNormalized(double Tolerance = SMALL_NUMBER) const;
    SVector2 Abs() const;

    double Dot(const SVector2& Other) const;
    SVector2 Cross(const SVector2& Other) const;
    SVector2 Rotated(const float Angle) const;

    glm::vec2 ToGLMVec2() const { return glm::vec2(X, Y); }
    std::string ToString() const;
};


struct SVector {
    double X;
    double Y;
    double Z;

    SVector() : X(0.0), Y(0.0), Z(0.0) {}
    SVector(double uniform) : X(uniform), Y(uniform), Z(uniform) {}
    SVector(double x, double y, double z) : X(x), Y(y), Z(z) {}

    SVector operator+(const SVector& Other) const;
    SVector operator+(double Scalar) const;
    SVector operator-(const SVector& Other) const;
    SVector operator-(double Scalar) const;

    SVector operator*(const SVector& Other) const;
    SVector operator*(double Scalar) const;
    SVector operator/(const SVector& Other) const;
    SVector operator/(double Scalar) const;

    SVector& operator+=(const SVector& Other);
    SVector& operator+=(double Scalar);
    SVector& operator-=(const SVector& Other);
    SVector& operator-=(double Scalar);

    SVector& operator*=(const SVector& Other);
    SVector& operator*=(double Scalar);
    SVector& operator/=(const SVector& Other);
    SVector& operator/=(double Scalar);

    double LengthSquared() const;
    double Length() const;

    SVector Normalized() const;
    SVector SafeNormalized(double Tolerance = SMALL_NUMBER) const;
    SVector Abs() const;

    double Dot(const SVector& Other) const;
    SVector Cross(const SVector& Other) const;
    SVector Rotated(const SRotator& Rotator) const;

    glm::vec3 ToGLMVec3() const;
    SRotator ToRotator() const;
    std::string ToString() const;
};

using SVector3 = SVector;
