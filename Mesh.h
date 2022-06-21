#pragma once
#include <vector>
#include "Vertex.h"
#include <string>
#include "MeshRenderData.h"
#include <memory>
#include <fstream>
class Mesh
{
public:
	std::vector<Vertex> rawVertices;
	std::vector<unsigned int> rawIndices;
	std::shared_ptr<MeshRenderData> renderData;
};

std::istream& operator>>(std::istream& is, Mesh& m);
std::ostream& operator<<(std::ostream& os, Mesh& m);