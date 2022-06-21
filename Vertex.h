#pragma once
#include <glm/glm.hpp>
#include <istream>
#include <ostream>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec3 Tangent;
};

std::istream& operator>>(std::istream& is, glm::vec3& v);
std::ostream& operator<<(std::ostream& os, const glm::vec3& v);
std::istream& operator>>(std::istream& is, glm::vec2& v);
std::ostream& operator<<(std::ostream& os, const glm::vec2& v);
std::istream& operator>>(std::istream& is, Vertex& v);
std::ostream& operator<<(std::ostream& os, const Vertex& v);
