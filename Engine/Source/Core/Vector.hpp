#pragma once

#include "Math.hpp"

#include <glm/glm.hpp>

#include <cmath>
#include <string>

struct SRotator;


struct SVector2 {
    float X;
    float Y;

    SVector2() : X(0), Y(0) {}
    SVector2(float uniform) : X(uniform), Y(uniform) {}
    SVector2(float x, float y) : X(x), Y(y) {}

    SVector2 operator+(const SVector2& Other) const;
    SVector2 operator+(float Scalar) const;
    SVector2 operator-(const SVector2& Other) const;
    SVector2 operator-(float Scalar) const;

    SVector2 operator*(const SVector2& Other) const;
    SVector2 operator*(float Scalar) const;
    SVector2 operator/(const SVector2& Other) const;
    SVector2 operator/(float Scalar) const;

    SVector2& operator+=(const SVector2& Other);
    SVector2& operator+=(float Scalar);
    SVector2& operator-=(const SVector2& Other);
    SVector2& operator-=(float Scalar);

    SVector2& operator*=(const SVector2& Other);
    SVector2& operator*=(float Scalar);
    SVector2& operator/=(const SVector2& Other);
    SVector2& operator/=(float Scalar);

    float LengthSquared() const;
    float Length() const;

    SVector2 Normalized() const;
    SVector2 SafeNormalized(float Tolerance = SMALL_NUMBER) const;
    SVector2 Abs() const;

    float Dot(const SVector2& Other) const;
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

    SVector2 XY() const { return SVector2(X, Y); }
    SVector2 XZ() const { return SVector2(X, Z); }
    SVector2 YZ() const { return SVector2(Y, Z); }

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


struct SVector4 {
    union {
        struct {
            float R;
            float G;
            float B;
            float A;
        };
        struct {
            float X;
            float Y;
            float Z;
            float W;
        };
    };

    SVector4() : X(0), Y(0), Z(0), W(0) {}
    SVector4(float uniform) : X(uniform), Y(uniform), Z(uniform), W(uniform) {}
    SVector4(float xy, float zw) : X(xy), Y(xy), Z(zw), W(zw) {}
    SVector4(float r, float g, float b, float a = 1.0f) : X(r), Y(g), Z(b), W(a) {}

    SVector2 XY() const { return SVector2(X, Y); }
    SVector2 XZ() const { return SVector2(X, Z); }
    SVector2 YW() const { return SVector2(Y, W); }
    SVector2 ZW() const { return SVector2(Z, W); }

    glm::vec4 ToGLMVec4() const { return glm::vec4(X, Y, Z, W); }
    std::string ToString() const;
};
