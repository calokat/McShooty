#pragma once
#include <glm/glm.hpp>
//#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	glm::vec3 Position;	    // The position of the vertex
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec3 Tangent;
};