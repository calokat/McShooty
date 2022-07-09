//
// Created by Caleb on 6/26/2022.
//

#ifndef HELLO_XR_MESHDESERIALIZER_H
#define HELLO_XR_MESHDESERIALIZER_H
#include "Mesh.h"
namespace MeshDeserializer
{
    void DeserializeMesh(Mesh& dest, std::vector<char> buffer);
    void DeserializeVertex(Vertex& v, std::vector<char>::iterator& it);
    void DeserializeVec3(glm::vec3& v, std::vector<char>::iterator& it);
    void DeserializeVec2(glm::vec2& v, std::vector<char>::iterator& it);
    void DeserializeFloat(float& f, std::vector<char>::iterator& it);
    void DeserializeUint(unsigned int &i, std::vector<char>::iterator& it);
}
#endif //HELLO_XR_MESHDESERIALIZER_H
