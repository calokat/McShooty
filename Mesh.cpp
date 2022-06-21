#include "Mesh.h"

std::istream& operator>>(std::istream& is, Mesh& m)
{
	unsigned int numVertices;
	is.read(reinterpret_cast<char*>(&numVertices), sizeof(unsigned int));
	std::vector<Vertex> vertices(numVertices);
	for (Vertex& v : vertices)
	{
		is >> v;
	}
	unsigned int numIndices;
	is.read(reinterpret_cast<char*>(&numIndices), sizeof(unsigned int));
	std::vector<unsigned int> indices(numIndices);
	for (unsigned int& i : indices)
	{
		is.read(reinterpret_cast<char*>(&i), sizeof(unsigned int));
	}
	m.rawIndices = indices;
	m.rawVertices = vertices;

	return is;
}

std::ostream& operator<<(std::ostream& os, Mesh& m)
{
	unsigned int numVertices = m.rawVertices.size();
	os.write(reinterpret_cast<char*>(&numVertices), sizeof(unsigned int));
	for (const Vertex& v : m.rawVertices)
	{
		os << v;
	}
	unsigned int numIndices = m.rawIndices.size();
	os.write(reinterpret_cast<char*>(&numIndices), sizeof(unsigned int));
	for (const unsigned int& i : m.rawIndices)
	{
		os.write(reinterpret_cast<const char*>(&i), sizeof(unsigned int));
	}
	return os;
}
