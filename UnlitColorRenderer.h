#pragma once
#include <gl/glew.h>
struct UnlitColorRenderer
{
    GLint attribInPosition;
    GLint uniColorTint;
    GLint uniModel;
    GLint uniView;
    GLint uniProjection;
};