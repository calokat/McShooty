#include "MeshLoaderSystem.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma warning(disable : 4996)

void MeshLoaderSystem::ProcessMesh(const char* path, Mesh& m)
{
	Assimp::Importer imp;
	const aiScene* meshScene = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_EmbedTextures);
	if (!meshScene->HasMeshes()) return;
	int meshIndex  = meshScene->mRootNode->mChildren[0]->mMeshes[0];
	aiMesh* assimpMesh = meshScene->mMeshes[meshIndex];
	for (unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.Position.x = assimpMesh->mVertices[i].x;
		vertex.Position.y = assimpMesh->mVertices[i].y;
		vertex.Position.z = assimpMesh->mVertices[i].z;
		if (assimpMesh->HasNormals())
		{
			vertex.Normal.x = assimpMesh->mNormals[i].x;
			vertex.Normal.y = assimpMesh->mNormals[i].y;
			vertex.Normal.z = assimpMesh->mNormals[i].z;
		}
		if (assimpMesh->mTextureCoords[0]) {
			vertex.UV.x = (float)assimpMesh->mTextureCoords[0][i].x;
			vertex.UV.y = (float)assimpMesh->mTextureCoords[0][i].y;
		}
		if (assimpMesh->HasTangentsAndBitangents())
		{
			vertex.Tangent.x = assimpMesh->mTangents[i].x;
			vertex.Tangent.y = assimpMesh->mTangents[i].y;
			vertex.Tangent.z = assimpMesh->mTangents[i].z;
		}
		m.rawVertices.push_back(vertex);
	}
	for (uint32_t i = 0; i < assimpMesh->mNumFaces; ++i)
	{
		aiFace face = assimpMesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; ++j)
		{
			m.rawIndices.push_back(face.mIndices[j]);
		}
	}
	m.path = assimpMesh->mName.C_Str();
	m.numIndices = m.rawIndices.size();
}