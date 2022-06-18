#include "OpenGLRenderSystem.h"
#include "MeshOpenGLRenderData.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

OpenGLRenderSystem::OpenGLRenderSystem()
{
    InitShaders();
}
void OpenGLRenderSystem::InstantiateRenderedObject(RenderedObject& ro)
{
    MeshOpenGLRenderData* meshRenderData = static_cast<MeshOpenGLRenderData*>(ro.mesh->renderData.get());
    glGenVertexArrays(1, &meshRenderData->vao);
    glBindVertexArray(meshRenderData->vao);
    glGenBuffers(1, &meshRenderData->vbo);
    glGenBuffers(1, &meshRenderData->ibo);
    glBindBuffer(GL_ARRAY_BUFFER, meshRenderData->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRenderData->ibo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * ro.mesh->rawVertices.size(), ro.mesh->rawVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ro.mesh->rawIndices.size(), ro.mesh->rawIndices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(unlitColorVars.attribInPosition);
    SetupAttribute(unlitColorVars.attribInPosition, 3, GL_FLOAT, Vertex, Position);
}

void OpenGLRenderSystem::LoadRenderCameraParams(const Camera& camera)
{
    cameraView = camera.view;
    cameraProjection = camera.projection;
}

void OpenGLRenderSystem::Draw(RenderedObject& ro)
{
    MeshOpenGLRenderData* renderData = static_cast<MeshOpenGLRenderData*>(ro.mesh->renderData.get());
    glBindVertexArray(renderData->vao);
    UpdateRenderer(ro);
    glDrawElements(GL_TRIANGLES, ro.mesh->rawIndices.size(), GL_UNSIGNED_INT, 0);
}

void OpenGLRenderSystem::InitShaders()
{
    programs->programID = glCreateProgram();
    programs->vertex = Shader("Shaders\\GLSL\\vertex-rainbow.glsl", GL_VERTEX_SHADER);
    programs->vertex.Compile();
    programs->fragment = Shader("Shaders\\GLSL\\fragment-rainbow.glsl", GL_FRAGMENT_SHADER);
    programs->fragment.Compile();
    glAttachShader(programs->programID, programs->vertex.GetId());
    glAttachShader(programs->programID, programs->fragment.GetId());
    glLinkProgram(programs->programID);
    glUseProgram(programs->programID);
    unlitColorVars.attribInPosition = glGetAttribLocation(programs->programID, "in_position");
    unlitColorVars.uniColorTint = glGetUniformLocation(programs->programID, "in_color");
    unlitColorVars.uniModel = glGetUniformLocation(programs->programID, "model");
    unlitColorVars.uniProjection = glGetUniformLocation(programs->programID, "projection");
    unlitColorVars.uniView = glGetUniformLocation(programs->programID, "view");
    unlitColorVars.uniTime = glGetUniformLocation(programs->programID, "time");
}

void OpenGLRenderSystem::UpdateRenderer(RenderedObject& ro)
{

    glUniformMatrix4fv(unlitColorVars.uniModel, 1, GL_FALSE, glm::value_ptr(ro.transform.worldMatrix));
    glUniformMatrix4fv(unlitColorVars.uniView, 1, GL_FALSE, glm::value_ptr(cameraView));
    glUniformMatrix4fv(unlitColorVars.uniProjection, 1, GL_FALSE, glm::value_ptr(cameraProjection));
    glm::vec4 rendererColor = ro.renderer.colorTint;
    glUniform4f(unlitColorVars.uniColorTint, rendererColor.r, rendererColor.g, rendererColor.b, rendererColor.a);
    glUniform1f(unlitColorVars.uniTime, ro.renderer.time);
}
