#pragma once

#include <limits>

struct SVector;


#define PI 3.14159265358979323846
#define TWO_PI (2.0 * PI)
#define HALF_PI (PI / 2.0)

#define SMALL_NUMBER 1.e-8
#define KINDA_SMALL_NUMBER 1.e-4
#define BIG_NUMBER 3.4e+38

const float FLOAT_INF = std::numeric_limits<float>::infinity();
const double DOUBLE_INF = std::numeric_limits<double>::infinity();

float ToRadians(float Degrees);
float ToDegrees(float Radians);

float Lerp(float A, float B, float Alpha);
double Lerp(double A, double B, double Alpha);

float InterpTo(float Current, float Target, float DeltaTime, float InterpSpeed);
float RInterpTo(float Current, float Target, float DeltaTime, float InterpSpeed);

// --- Collision Helpers ---

bool CheckRayOBB(
    const SVector& Start, const SVector& Dir, float MaxDist, const SVector& BoxCenter, const SVector& BoxExtents,
    const SVector& AX, const SVector& AY, const SVector& AZ, float& OutDist, SVector& OutNormal
);

bool CheckRayTriangle(
    const SVector& Start, const SVector& Dir, const SVector& V0, const SVector& V1, const SVector& V2, float& OutDist,
    SVector& OutNormal
);
