#pragma once

#include <glm/glm.hpp>

#include <cmath>
#include <string>

struct SRotator;


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

    SVector GetNormalized() const;
    SVector GetSafeNormalized(double Tolerance = 1.e-8) const;
    SVector GetAbs() const;

    double Dot(const SVector& Other) const;
    SVector Cross(const SVector& Other) const;
    SVector GetRotated(const SRotator& Rotator) const;

    glm::vec3 ToGLMVec3() const;
    std::string ToString() const;
};
