#include <stdio.h>
#ifdef _WIN64
#include "WindowsPlatform.h"
#include "WindowsAssetManager.h"
#endif
#ifdef __linux__
#include "LinuxPlatform.h"
#endif
#include "OpenGLAPI.h"
#include "MeshDeserializer.h"
#include "RenderedObject.h"
#include "MeshOpenGLRenderData.h"
#include "TransformSystem.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "OpenGLRenderSystem.h"
#include "OpenXrApi.h"
#include "SteamVrApi.h"
#include <chrono>
#include <fstream>
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
    OpenXrApi xr(platform, graphics, PE::GraphicsAPI::OpenGL);
#ifdef _WIN32
    WindowsAssetManager assetManager;
#endif
    //SteamVrApi vr;
    //RenderedObject spiral, torus;
    std::vector<RenderedObject> objectsToRender(3);
    RenderedObject& spiral = objectsToRender[0];
    RenderedObject& leftHand = objectsToRender[1];
    RenderedObject& rightHand = objectsToRender[2];
    spiral.renderer.colorTint = { 1, 0, 1, 1 };
    spiral.transform.scale = glm::vec3(5);
    leftHand.renderer.colorTint = { 0, 0, 0, 1 };
    rightHand.renderer.colorTint = { 1, 1, 1, 1 };
    Camera camera(8.0f / 6);
    Transform cameraTransform; 
    TransformSystem::CalculateWorldMatrix(&cameraTransform);
    CameraSystem::CalculateViewMatrixLH(camera, cameraTransform);
    CameraSystem::CalculateProjectionMatrixLH(camera, camera.aspectRatio);
    std::shared_ptr<Mesh> spiralMesh = std::make_shared<Mesh>();
    std::shared_ptr<Mesh> handMesh = std::make_shared<Mesh>();
    spiralMesh->renderData = std::make_shared<MeshOpenGLRenderData>();
    handMesh->renderData = std::make_shared<MeshOpenGLRenderData>();

    Asset spiralAsset = assetManager.LoadAsset("Models\\helix.mesh");
    MeshDeserializer::DeserializeMesh(*spiralMesh, spiralAsset.GetBytes());

    Asset handAsset = assetManager.LoadAsset("Models\\cube.mesh");
    MeshDeserializer::DeserializeMesh(*handMesh, handAsset.GetBytes());

    spiral.mesh = spiralMesh;
    leftHand.mesh = handMesh;
    rightHand.mesh = handMesh;
    OpenGLRenderSystem renderSystem(assetManager);
    renderSystem.LoadRenderCameraParams(camera);
    spiral.transform.orientation = glm::quat(glm::vec3(0, 0, glm::radians(90.0f)));

    TransformSystem::CalculateWorldMatrix(&spiral.transform);
    TransformSystem::CalculateWorldMatrix(&leftHand.transform);
    TransformSystem::CalculateWorldMatrix(&rightHand.transform);
    renderSystem.InstantiateRenderedObject(spiral);
    renderSystem.InstantiateRenderedObject(leftHand);
    renderSystem.InstantiateRenderedObject(rightHand);


    // with thanks to https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c
    double elapsed_time_ms = 0;
    float totalTime = 0;
    while (platform.Run())
    {
        auto t_start = std::chrono::high_resolution_clock::now();
        if (totalTime > 10) totalTime -= 10;
        spiral.renderer.time = totalTime;
        leftHand.renderer.time = totalTime;
        rightHand.renderer.time = totalTime;
        //vr.UpdateDevices();
        //leftHand.transform = vr.GetTrackedDeviceTransform(ETrackedDeviceType::LeftHandController);
        //rightHand.transform = vr.GetTrackedDeviceTransform(ETrackedDeviceType::RightHandController);
        spiral.transform.orientation = spiral.transform.orientation * glm::angleAxis((float)elapsed_time_ms / 1000, glm::vec3(0, 1, 0));
        TransformSystem::CalculateWorldMatrix(&spiral.transform);
        //vr.DrawFrame(objectsToRender, renderSystem);
        xr.Frame(objectsToRender, renderSystem, cameraTransform);
        graphics.ClearScreen();
        //renderSystem.Draw(spiral);
        //graphics._SwapBuffers();
        auto t_end = std::chrono::high_resolution_clock::now();
        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
        totalTime += (elapsed_time_ms / 1000.0f);
    }
    return 0;
}