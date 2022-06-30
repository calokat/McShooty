#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void LoadMesh(Mesh& m, const char* meshPath)
{
	Assimp::Importer importer;
	const aiScene* meshScene = importer.ReadFile(meshPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_EmbedTextures);
	int meshIndex = meshScene->mRootNode->mChildren[0]->mMeshes[0];
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
}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
		Mesh toExport;
        const char* meshPath = argv[i];
		LoadMesh(toExport, meshPath);
		std::string processedPath(meshPath);
		size_t lastDotPos = processedPath.find_last_of(".");
		std::string substring = processedPath.substr(0, lastDotPos);
		substring.append(".mesh");
		std::ofstream outFile(substring.c_str(), std::ios_base::binary);
		outFile << toExport;
    }
}