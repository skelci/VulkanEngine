#pragma once

#include <glm/glm.hpp>

#include <string>


struct SColor {
    float R;
    float G;
    float B;
    float A;

    SColor() : R(1), G(1), B(1), A(1) {}
    SColor(float uniform) : R(uniform), G(uniform), B(uniform), A(uniform) {}
    SColor(float r, float g, float b, float a = 1.0f) : R(r), G(g), B(b), A(a) {}

    glm::vec4 ToVec4() const { return glm::vec4(R, G, B, A); }
    std::string ToString() const;
};
