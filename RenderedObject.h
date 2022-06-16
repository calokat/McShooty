#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "UnlitColorRenderer.h"

struct RenderedObject
{
    std::shared_ptr<Mesh> mesh;
    Transform transform;
    UnlitColorRenderer renderer;
};