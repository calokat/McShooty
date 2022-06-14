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
#include "openvr.h"

#define SetupAttribute(index, size, type, structure, element) \
	glVertexAttribPointer(index, size, type, 0, sizeof(structure), (void*)offsetof(structure, element)); \

struct ControllerInfo_t
{
    vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
    vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
    vr::VRActionHandle_t m_actionHaptic = vr::k_ulInvalidActionHandle;
};

enum EHand
{
    Left = 0,
    Right = 1,
};

struct FramebufferDesc
{
    GLuint m_nDepthBufferId;
    GLuint m_nRenderTextureId;
    GLuint m_nRenderFramebufferId;
    GLuint m_nResolveTextureId;
    GLuint m_nResolveFramebufferId;
};

bool CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc& framebufferDesc)
{
    glGenFramebuffers(1, &framebufferDesc.m_nRenderFramebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nRenderFramebufferId);

    glGenRenderbuffers(1, &framebufferDesc.m_nDepthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, nWidth, nHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);

    glGenTextures(1, &framebufferDesc.m_nRenderTextureId);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, nWidth, nHeight, true);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId, 0);

    glGenFramebuffers(1, &framebufferDesc.m_nResolveFramebufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nResolveFramebufferId);

    glGenTextures(1, &framebufferDesc.m_nResolveTextureId);
    glBindTexture(GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId, 0);

    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

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
    vr::EVRInitError initError = vr::EVRInitError::VRInitError_None;
    vr::IVRSystem* m_pHMD = vr::VR_Init(&initError, vr::VRApplication_Scene);
    vr::VRCompositor();
    uint32_t m_nRenderWidth = 0;
    uint32_t m_nRenderHeight = 0;
    FramebufferDesc leftEyeDesc;
    FramebufferDesc rightEyeDesc;

    m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);
    CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, leftEyeDesc);
    CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc);

    vr::VRInput()->SetActionManifestPath("C:/Users/Caleb/Documents/GitHub/McShooty/build/out/Assets/hellovr_actionsbvnmnb.json");
    vr::VRActionHandle_t m_actionHideCubes, m_actionHideThisController, m_actionTriggerHaptic, m_actionAnalongInput;
    vr::VRActionSetHandle_t m_actionsetDemo;
    ControllerInfo_t m_rHand[2];
    vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    vr::VRInput()->GetActionHandle("/actions/demo/in/HideCubes", &m_actionHideCubes);
    vr::VRInput()->GetActionHandle("/actions/demo/in/HideThisController", &m_actionHideThisController);
    vr::VRInput()->GetActionHandle("/actions/demo/in/TriggerHaptic", &m_actionTriggerHaptic);
    vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_actionAnalongInput);

    vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);

    vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Left", &m_rHand[Left].m_actionHaptic);
    vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
    vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_rHand[Left].m_actionPose);

    vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Right", &m_rHand[Right].m_actionHaptic);
    vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);
    vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_rHand[Right].m_actionPose);

    //RenderedObject spiral, torus;
    std::vector<RenderedObject> objectsToRender(2);
    RenderedObject& spiral = objectsToRender[0];
    RenderedObject& torus = objectsToRender[1];
    spiral.renderer.colorTint = { 1, 0, 1, 1 };
    torus.renderer.colorTint = { 1, .6f, 0, 1 };
    Camera camera(8.0f / 6);
    Transform cameraTransform; 
    TransformSystem::CalculateWorldMatrix(&cameraTransform);
    CameraSystem::CalculateViewMatrixLH(camera, cameraTransform);
    CameraSystem::CalculateProjectionMatrixLH(camera, camera.aspectRatio);
    MeshLoaderSystem::ProcessMesh("Models\\helix.obj", spiral.mesh);
    MeshLoaderSystem::ProcessMesh("Models\\helix.obj", torus.mesh);
    spiral.mesh.renderData = std::make_unique<MeshOpenGLRenderData>();
    torus.mesh.renderData = std::make_unique<MeshOpenGLRenderData>();
    OpenGLRenderSystem renderSystem;
    renderSystem.LoadRenderCameraParams(camera);
    spiral.transform.position = glm::vec3(0, 0, 15);
    torus.transform.position = glm::vec3(0, 5, 15);
    TransformSystem::CalculateWorldMatrix(&spiral.transform);
    TransformSystem::CalculateWorldMatrix(&torus.transform);
    renderSystem.InstantiateRenderedObject(spiral);
    renderSystem.InstantiateRenderedObject(torus);
    
    while (platform.Run())
    {
        vr::VREvent_t event;
        while (m_pHMD->PollNextEvent(&event, sizeof(event)))
        {
        }

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_MULTISAMPLE);

        // Left Eye
        glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
        glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
        //RenderScene(vr::Eye_Left);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_MULTISAMPLE);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEyeDesc.m_nResolveFramebufferId);

        glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
            GL_COLOR_BUFFER_BIT,
            GL_LINEAR);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glEnable(GL_MULTISAMPLE);

        // Right Eye
        glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
        glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
        //RenderScene(vr::Eye_Right);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_MULTISAMPLE);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFramebufferId);

        glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
            GL_COLOR_BUFFER_BIT,
            GL_LINEAR);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)leftEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
        vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)rightEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
        vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);


        vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

        graphics.ClearScreen();
        renderSystem.Draw(spiral);
        renderSystem.Draw(torus);
        graphics._SwapBuffers();
        //xr.Frame(objectsToRender, renderSystem, cameraTransform);
    }
    return 0;
}