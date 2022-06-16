#include <stdio.h>
#ifdef _WIN64
#include "WindowsPlatform.h"
#endif
#ifdef __linux__
#include "LinuxPlatform.h"
#endif
#include "OpenGLAPI.h"
#include "MeshLoaderSystem.h"
#include "RenderedObject.h"
#include "MeshOpenGLRenderData.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "OpenGLRenderSystem.h"
#include "OpenXrApi.h"
#include "SteamVrApi.h"

#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \


int main(int argc, char* argv[])
{
#ifdef _WIN32
    WindowsPlatform platform;
    SetCurrentDirectory("..\\Assets");
#elif defined __linux__
    LinuxPlatform platform;
    chdir("../Assets");
#endif
    platform.InitWindow();
#ifdef _WIN32
    WinOpenGLContext winGlContext(platform);
#endif
    OpenGLAPI graphics(winGlContext);
    graphics.Init();
    printf("Welcome to McShooty's\n");
    //OpenXrApi xr(platform, graphics, PE::GraphicsAPI::OpenGL);
    SteamVrApi vr;
    //RenderedObject spiral, torus;
    std::vector<RenderedObject> objectsToRender(2);
    RenderedObject& spiral = objectsToRender[0];
    RenderedObject& leftHand = objectsToRender[1];
    spiral.renderer.colorTint = { 1, 0, 1, 1 };
    leftHand.renderer.colorTint = { 0, 0, 0, 1 };
    Camera camera(8.0f / 6);
    Transform cameraTransform; 
    TransformSystem::CalculateWorldMatrix(&cameraTransform);
    CameraSystem::CalculateViewMatrixLH(camera, cameraTransform);
    CameraSystem::CalculateProjectionMatrixLH(camera, camera.aspectRatio);
    std::shared_ptr<Mesh> spiralMesh = std::make_shared<Mesh>();
    std::shared_ptr<Mesh> handMesh = std::make_shared<Mesh>();
    spiralMesh->renderData = std::make_shared<MeshOpenGLRenderData>();
    handMesh->renderData = std::make_shared<MeshOpenGLRenderData>();
    MeshLoaderSystem::ProcessMesh("Models\\cubeHand.obj", *handMesh);
    MeshLoaderSystem::ProcessMesh("Models\\helix.obj", *spiralMesh);
    spiral.mesh = spiralMesh;
    leftHand.mesh = handMesh;
    OpenGLRenderSystem renderSystem;
    renderSystem.LoadRenderCameraParams(camera);
    spiral.transform.orientation = glm::quat(glm::vec3(0, 0, glm::radians(90.0f)));

    TransformSystem::CalculateWorldMatrix(&spiral.transform);
    TransformSystem::CalculateWorldMatrix(&leftHand.transform);
    renderSystem.InstantiateRenderedObject(spiral);
    renderSystem.InstantiateRenderedObject(leftHand);

    while (platform.Run())
    {
        vr.UpdateDevices();
        leftHand.transform = vr.GetTrackedDeviceTransform(ETrackedDeviceType::LeftHandController);
        vr.DrawFrame(objectsToRender, renderSystem);
        graphics.ClearScreen();
        renderSystem.Draw(spiral);
        graphics._SwapBuffers();
        //xr.Frame(objectsToRender, renderSystem, cameraTransform);
    }
    return 0;
}