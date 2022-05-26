#include <stdio.h>
#include "WindowsPlatform.h"
#include "OpenGLAPI.h"
#include "MeshLoaderSystem.h"
#include "Shader.h"
#include "RenderedObject.h"
#include "MeshOpenGLRenderData.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"

#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \


int main(int argc, char* argv[])
{
    WindowsPlatform platform;
    OpenGLAPI graphics(&platform);
    platform.InitWindow();
    graphics.Init();
    printf("Welcome to McShooty's\n");
    RenderedObject spiral;
    Camera camera(8.0f / 6);
    Transform cameraTransform;
    TransformSystem::CalculateWorldMatrix(&cameraTransform);
    CameraSystem::CalculateViewMatrixLH(camera, cameraTransform);
    CameraSystem::CalculateProjectionMatrixLH(camera, camera.aspectRatio);
    MeshLoaderSystem::ProcessMesh("C:\\Users\\Caleb\\Documents\\GitHub\\PortableEngine\\Assets\\Models\\helix.obj", spiral.mesh);
    spiral.mesh.renderData = std::make_unique<MeshOpenGLRenderData>();
    Shader vertex("C:/Users/Caleb/Documents/GitHub/McShooty/vertex-unlit-color.glsl", GL_VERTEX_SHADER);
    vertex.Compile();
    Shader fragment("C:/Users/Caleb/Documents/GitHub/McShooty/fragment-unlit-color.glsl", GL_FRAGMENT_SHADER);
    fragment.Compile();
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex.GetId());
    glAttachShader(program, fragment.GetId());
    glLinkProgram(program);
    glUseProgram(program);
    MeshOpenGLRenderData* renderData = static_cast<MeshOpenGLRenderData*>(spiral.mesh.renderData.get());
    glGenVertexArrays(1, &renderData->vao);
    glBindVertexArray(renderData->vao);
    glGenBuffers(1, &renderData->vbo);
    glGenBuffers(1, &renderData->ibo);
    glBindBuffer(GL_ARRAY_BUFFER, renderData->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderData->ibo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * spiral.mesh.rawVertices.size(), spiral.mesh.rawVertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * spiral.mesh.rawIndices.size(), spiral.mesh.rawIndices.data(), GL_STATIC_DRAW);

    GLenum err = glGetError();
    GLint positionAttribLocation = glGetAttribLocation(program, "in_position");
    err = glGetError();
    glEnableVertexAttribArray(positionAttribLocation);
    err = glGetError();
    SetupAttribute(positionAttribLocation, 3, GL_FLOAT, Vertex, Position);
    err = glGetError();

    spiral.transform.position = glm::vec3(0, 0, 15);
    TransformSystem::CalculateWorldMatrix(&spiral.transform);

    GLint colorLocation = glGetUniformLocation(program, "in_color");
    err = glGetError();
    GLint modelLocation = glGetUniformLocation(program, "model");
    GLint viewLocation = glGetUniformLocation(program, "view");
    GLint projectionLocation = glGetUniformLocation(program, "projection");
    err = glGetError();

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(spiral.transform.worldMatrix));
    err = glGetError();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.projection));
    glUniform4f(colorLocation, 1, 0, 0, 1);
    err = glGetError();


    while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) 
    {
        platform.Run();
        err = glGetError();

        graphics.ClearScreen();
        glDrawElements(GL_TRIANGLES, spiral.mesh.rawIndices.size(), GL_UNSIGNED_INT, 0);
        graphics._SwapBuffers();
    }
    return 0;
}