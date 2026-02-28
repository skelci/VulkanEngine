#include "Math.hpp"

#include "Vector.hpp"

#include <algorithm>
#include <cmath>


float ToRadians(float Degrees) { return Degrees * (PI / 180.0f); }

float ToDegrees(float Radians) { return Radians * (180.0f / PI); }

float Lerp(float A, float B, float Alpha) { return A + Alpha * (B - A); }

double Lerp(double A, double B, double Alpha) { return A + Alpha * (B - A); }

float InterpTo(float Current, float Target, float DeltaTime, float InterpSpeed) {
    const float InterpDelta = Target - Current;
    return Current + std::clamp(InterpDelta, -InterpSpeed * DeltaTime, InterpSpeed * DeltaTime);
}

float RInterpTo(float Current, float Target, float DeltaTime, float InterpSpeed) {
    float Delta = Target - Current;

    Delta = std::fmod(Delta, 360.0f);
    if (Delta > 180.0f) {
        Delta -= 360.0f;
    } else if (Delta < -180.0f) {
        Delta += 360.0f;
    }

    const float MaxStep = InterpSpeed * DeltaTime;

    return Current + std::clamp(Delta, -MaxStep, MaxStep);
}

// --- Collision Helpers ---

bool CheckRayOBB(
    const SVector& Start, const SVector& Dir, float MaxDist, const SVector& BoxCenter, const SVector& BoxExtents,
    const SVector& AX, const SVector& AY, const SVector& AZ, float& OutDist, SVector& OutNormal
) {
    double tMin = 0.0;
    double tMax = MaxDist;
    SVector CurrentNormal = SVector(0);

    SVector P_C = BoxCenter - Start;
    SVector Axes[3] = {AX, AY, AZ};
    double Ext[3] = {BoxExtents.X, BoxExtents.Y, BoxExtents.Z};

    for (int i = 0; i < 3; i++) {
        double e = Ext[i];
        double d = Dir.Dot(Axes[i]);
        double p = P_C.Dot(Axes[i]);

        if (std::abs(d) < SMALL_NUMBER) {
            if (-p - e > 0 || -p + e < 0) return false;
        } else {
            double t1 = (p - e) / d;
            double t2 = (p + e) / d;
            SVector n1 = -Axes[i];
            SVector n2 = Axes[i];

            if (t1 > t2) {
                std::swap(t1, t2);
                std::swap(n1, n2);
            }

            if (t1 > tMin) {
                tMin = t1;
                CurrentNormal = n1;
            }
            if (t2 < tMax) {
                tMax = t2;
            }

            if (tMin > tMax) return false;
        }
    }

    OutDist = tMin;
    OutNormal = CurrentNormal;
    return true;
}

bool CheckRayTriangle(
    const SVector& Start, const SVector& Dir, const SVector& V0, const SVector& V1, const SVector& V2, float& OutDist,
    SVector& OutNormal
) {
    const double EPSILON = SMALL_NUMBER;
    SVector edge1 = V1 - V0;
    SVector edge2 = V2 - V0;
    SVector h = Dir.Cross(edge2);
    double a = edge1.Dot(h);

    if (a > -EPSILON && a < EPSILON) return false;

    double f = 1.0 / a;
    SVector s = Start - V0;
    double u = f * s.Dot(h);

    if (u < 0.0 || u > 1.0) return false;

    SVector q = s.Cross(edge1);
    double v = f * Dir.Dot(q);

    if (v < 0.0 || u + v > 1.0) return false;

    double t = f * edge2.Dot(q);

    if (t > EPSILON) {
        OutDist = t;
        OutNormal = edge1.Cross(edge2).Normalized();
        if (OutNormal.Dot(Dir) > 0) OutNormal = -OutNormal;
        return true;
    }

    return false;
}
