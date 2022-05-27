#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "UnlitColorRenderer.h"

struct RenderedObject
{
    Mesh mesh;
    Transform transform;
    UnlitColorRenderer renderer;
};