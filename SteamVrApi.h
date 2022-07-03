#ifdef _WIN32
#pragma once
#include "openvr.h"
#include <glm/glm.hpp>
#include <gl/glew.h>
#include "Camera.h"
#include <vector>
#include "RenderedObject.h"
#include "OpenGLRenderSystem.h"

struct FramebufferDesc
{
    GLuint m_nDepthBufferId;
    GLuint m_nRenderTextureId;
    GLuint m_nRenderFramebufferId;
    GLuint m_nResolveTextureId;
    GLuint m_nResolveFramebufferId;
};

struct ControllerInfo_t
{
    vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
    vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
    vr::VRActionHandle_t m_actionHaptic = vr::k_ulInvalidActionHandle;
    glm::mat4 m_rmat4Pose;
};

enum EHand
{
    Left = 0,
    Right = 1,
};

enum ETrackedDeviceType
{
    Hmd,
    LeftHandController,
    RightHandController,
    __COUNT__
};

class SteamVrApi
{
    glm::mat4 ConvertToGlmMatrix4(const vr::HmdMatrix34_t& vrMatrix);
    glm::mat4 ConvertToGlmMatrix4(const vr::HmdMatrix44_t& vrMatrix);
    bool CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc& framebufferDesc);
    uint32_t m_nRenderWidth = 0;
    uint32_t m_nRenderHeight = 0;
    FramebufferDesc leftEyeDesc;
    FramebufferDesc rightEyeDesc;
    vr::VRActionHandle_t m_actionHideCubes, m_actionHideThisController, m_actionTriggerHaptic, m_actionAnalongInput;
    vr::VRActionSetHandle_t m_actionsetDemo;
    vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    ControllerInfo_t m_rHand[2];
    Camera eyes[2];
    glm::mat4 m_mat4HMDPose;
    vr::IVRSystem* m_pHMD;
    Transform trackedDeviceTransforms[ETrackedDeviceType::__COUNT__];
public:
    SteamVrApi();
    void UpdateDevices();
    void DrawFrame(std::vector<RenderedObject>& objects, OpenGLRenderSystem& renderSystem);
    const Transform& GetTrackedDeviceTransform(ETrackedDeviceType type);
};
#endif