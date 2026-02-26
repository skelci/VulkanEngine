#pragma once

#include <limits>


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

int Clamp(int Value, int Min, int Max);
float Clamp(float Value, float Min, float Max);
double Clamp(double Value, double Min, double Max);
