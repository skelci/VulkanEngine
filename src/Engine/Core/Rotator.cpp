#include "Rotator.hpp"

glm::vec3 SRotator::GetForwardVector() const {
    float CP = glm::cos(glm::radians(Pitch));
    float SP = glm::sin(glm::radians(Pitch));
    float CY = glm::cos(glm::radians(Yaw));
    float SY = glm::sin(glm::radians(Yaw));

    return glm::vec3(CP * CY, CP * SY, SP);
}
