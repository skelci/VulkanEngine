#include "Random.hpp"

#include "Math.hpp"

#include <random>


double Random::Rand() { return static_cast<double>(rand()) / RAND_MAX; }

int32 Random::RandRange(int32 Min, int32 Max) { return Min + (Max - Min) * Rand(); }

double Random::RandRange(double Min, double Max) { return Min + (Max - Min) * Rand(); }
