#pragma once
#include "MeshRenderData.h"
#include <GL/gl.h>

class MeshOpenGLRenderData: public MeshRenderData
{
public:
    GLuint vao, vbo, ibo;
};