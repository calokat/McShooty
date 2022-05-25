#pragma once
#include "Vertex.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace MeshLoaderSystem
{
	void ProcessMesh(const char* path, Mesh& m);
};
