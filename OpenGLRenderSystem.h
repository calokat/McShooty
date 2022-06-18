#pragma once
#include "RenderedObject.h"
#include "OpenGLShaderProgram.h"
#include "Camera.h"

#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \


class OpenGLRenderSystem
{
public:
    OpenGLRenderSystem();
    void InstantiateRenderedObject(RenderedObject& ro);
    void LoadRenderCameraParams(const Camera& camera);
    void Draw(RenderedObject& ro);
private:
    struct UnlitColorShaderData
    {
        GLint attribInPosition;
        GLint uniColorTint;
        GLint uniModel;
        GLint uniView;
        GLint uniProjection;
        GLint uniTime;
    } unlitColorVars;
    void InitShaders();
    void UpdateRenderer(RenderedObject& ro);
    OpenGLShaderProgram programs[1];
    glm::mat4 cameraView;
    glm::mat4 cameraProjection;
};