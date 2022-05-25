#pragma once
#include <vector>
#include "Vertex.h"
#include <string>
class Mesh
{
public:
	int numIndices;
	std::string path;
	std::vector<Vertex> rawVertices;
	std::vector<unsigned int> rawIndices;
};
