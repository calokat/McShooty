#pragma once

#include <GL/glew.h>
#include "Shader.h"

struct OpenGLShaderProgram
{
    Shader vertex;
    Shader fragment;
    GLuint programID;
};