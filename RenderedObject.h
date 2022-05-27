#pragma once
#include "Mesh.h"
#include "Transform.h"

struct RenderedObject
{
    Mesh mesh;
    Transform transform;
    UnlitColorRenderer renderer;
};