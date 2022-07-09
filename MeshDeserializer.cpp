//
// Created by Caleb on 6/26/2022.
//
#include "MeshDeserializer.h"

void MeshDeserializer::DeserializeMesh(Mesh &dest, std::vector<char> buffer)
{
    auto it = buffer.begin();
    unsigned int numVertices = 0;
    DeserializeUint(numVertices, it);
    dest.rawVertices = std::vector<Vertex>(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        DeserializeVertex(dest.rawVertices[i], it);
    }
    unsigned int numIndices;
    DeserializeUint(numIndices, it);
    dest.rawIndices = std::vector<unsigned int>(numIndices);
    for (int i = 0; i < numIndices; ++i)
    {
        DeserializeUint(dest.rawIndices[i], it);
    }
}

void MeshDeserializer::DeserializeVertex(Vertex &v, std::vector<char>::iterator& it)
{
    DeserializeVec3(v.Position, it);
    DeserializeVec3(v.Normal, it);
    DeserializeVec2(v.UV, it);
    DeserializeVec3(v.Tangent, it);
}

void MeshDeserializer::DeserializeVec3(glm::vec3 &v, std::vector<char>::iterator& it)
{
    DeserializeFloat(v.x, it);
    DeserializeFloat(v.y, it);
    DeserializeFloat(v.z, it);
}

void MeshDeserializer::DeserializeVec2(glm::vec2 &v, std::vector<char>::iterator& it)
{
    DeserializeFloat(v.x, it);
    DeserializeFloat(v.y, it);
}

void MeshDeserializer::DeserializeFloat(float &f, std::vector<char>::iterator &it)
{
    memcpy(&f, &(*it), sizeof(float));
    it += sizeof(float);
}

void MeshDeserializer::DeserializeUint(unsigned int &i, std::vector<char>::iterator &it)
{
    memcpy(&i, &(*it), sizeof(unsigned int));
    it += sizeof(unsigned int);
}