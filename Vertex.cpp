#include "Vertex.h"

std::istream& operator>>(std::istream& is, glm::vec3& v)
{
	float x, y, z;
	is.read(reinterpret_cast<char*>(&x), sizeof(float));
	is.read(reinterpret_cast<char*>(&y), sizeof(float));
	is.read(reinterpret_cast<char*>(&z), sizeof(float));
	v = { x, y, z };
	return is;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
	os.write(reinterpret_cast<const char*>(&v.x), sizeof(float));
	os.write(reinterpret_cast<const char*>(&v.y), sizeof(float));
	os.write(reinterpret_cast<const char*>(&v.z), sizeof(float));
	return os;
}

std::istream& operator>>(std::istream& is, glm::vec2& v)
{
	float x, y;
	is.read(reinterpret_cast<char*>(&x), sizeof(float));
	is.read(reinterpret_cast<char*>(&y), sizeof(float));
	v = { x, y };
	return is;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
{
	os.write(reinterpret_cast<const char*>(&v.x), sizeof(float));
	os.write(reinterpret_cast<const char*>(&v.y), sizeof(float));
	return os;
}

std::istream& operator>>(std::istream& is, Vertex& v)
{
	return is >> v.Position >> v.Normal >> v.UV >> v.Tangent;
}

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
	return os << v.Position << v.Normal << v.UV << v.Tangent;
}
