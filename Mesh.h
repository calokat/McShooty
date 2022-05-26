#pragma once
#include <vector>
#include "Vertex.h"
#include <string>
#include "MeshRenderData.h"
#include <memory>

class Mesh
{
public:
	int numIndices;
	std::string path;
	std::vector<Vertex> rawVertices;
	std::vector<unsigned int> rawIndices;
	std::unique_ptr<MeshRenderData> renderData;
};
