#include "Math.hpp"

inline float ToRadians(float Degrees) {
    return Degrees * (PI / 180.0);
}

inline float ToDegrees(float Radians) {
    return Radians * (180.0 / PI);
}

inline float Lerp(float A, float B, float Alpha) {
    return A + Alpha * (B - A);
}

inline double Lerp(double A, double B, double Alpha) {
    return A + Alpha * (B - A);
}

inline int Clamp(int Value, int Min, int Max) {
    if (Value < Min) return Min;
    if (Value > Max) return Max;
    return Value;
}

inline float Clamp(float Value, float Min, float Max) {
    if (Value < Min) return Min;
    if (Value > Max) return Max;
    return Value;
}

inline double Clamp(double Value, double Min, double Max) {
    if (Value < Min) return Min;
    if (Value > Max) return Max;
    return Value;
}
