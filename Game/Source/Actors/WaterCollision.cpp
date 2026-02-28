#include "WaterCollision.hpp"


AWaterCollision::AWaterCollision() {
    SetBoxExtent(SVector(1, 1, 1));
    Transform.Position.Z = -1;
}
