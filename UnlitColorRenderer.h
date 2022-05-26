#pragma once
#include "glm/glm.hpp"

struct UnlitColorRenderer
{
    glm::vec3 colorTint;
    glm::mat4 world;
    glm::mat4 view;
    glm::mat4 projection;
};