#include "Color.hpp"


std::string SColor::ToString() const {
    return "SColor(" + std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + ", " +
           std::to_string(A) + ")";
}
