#include <stdio.h>
#ifdef _WIN64
#include "WindowsPlatform.h"
#endif
#ifdef __linux__
#include "LinuxPlatform.h"
#endif
#include "OpenGLAPI.h"
#include "MeshLoaderSystem.h"
#include "Shader.h"
#include "RenderedObject.h"
#include "MeshOpenGLRenderData.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "OpenGLRenderSystem.h"

#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \

bool EventLoop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_ESCAPE)
            {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    LinuxPlatform platform;
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
    MeshLoaderSystem::ProcessMesh("~/git/PortableEngine/Assets/Models/helix.obj", spiral.mesh);
    spiral.mesh.renderData = std::make_unique<MeshOpenGLRenderData>();
    OpenGLRenderSystem renderSystem;
    renderSystem.LoadRenderCameraParams(camera);
    spiral.transform.position = glm::vec3(0, 0, 15);
    TransformSystem::CalculateWorldMatrix(&spiral.transform);
    renderSystem.InstantiateRenderedObject(spiral);
    
    while (EventLoop())
    {
        platform.Run();
        graphics.ClearScreen();
        glDrawElements(GL_TRIANGLES, spiral.mesh.rawIndices.size(), GL_UNSIGNED_INT, 0);
        graphics._SwapBuffers();
    }
    return 0;
}