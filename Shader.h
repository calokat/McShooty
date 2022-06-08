#pragma once
#include <string>
#include "GL/glew.h"

class Shader
{
private:
	GLint shaderType;
	GLint id;
public:
	Shader();
	Shader(std::string pathArg, GLint typeArg);
	void Compile();
	GLint GetId();
};

