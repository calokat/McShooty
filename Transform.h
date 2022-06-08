#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Transform
{
	glm::mat4 worldMatrix = {};
	glm::vec3 position = {};
	glm::vec3 rotation = {};
	glm::vec3 scale = {1, 1, 1};
	glm::quat orientation = glm::identity<glm::quat>();
};