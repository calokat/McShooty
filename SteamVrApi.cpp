#include "SteamVrApi.h"
#include "TransformSystem.h"
glm::mat4 SteamVrApi::ConvertToGlmMatrix4(const vr::HmdMatrix34_t& vrMatrix)
{
    return {
        {vrMatrix.m[0][0], vrMatrix.m[1][0], vrMatrix.m[2][0], 0.0},
        {vrMatrix.m[0][1], vrMatrix.m[1][1], vrMatrix.m[2][1], 0.0},
        {vrMatrix.m[0][2], vrMatrix.m[1][2], vrMatrix.m[2][2], 0.0},
        {vrMatrix.m[0][3], vrMatrix.m[1][3], vrMatrix.m[2][3], 1.0}
    };
}

glm::mat4 SteamVrApi::ConvertToGlmMatrix4(const vr::HmdMatrix44_t& vrMatrix)
{
    return {
        {vrMatrix.m[0][0], vrMatrix.m[1][0], vrMatrix.m[2][0], vrMatrix.m[3][0]},
        {vrMatrix.m[0][1], vrMatrix.m[1][1], vrMatrix.m[2][1], vrMatrix.m[3][1]},
        {vrMatrix.m[0][2], vrMatrix.m[1][2], vrMatrix.m[2][2], vrMatrix.m[3][2]},
        {vrMatrix.m[0][3], vrMatrix.m[1][3], vrMatrix.m[2][3], vrMatrix.m[3][3]}
    };
}

bool SteamVrApi::CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc& framebufferDesc)
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

SteamVrApi::SteamVrApi()
{
    vr::EVRInitError initError = vr::EVRInitError::VRInitError_None;
    m_pHMD = vr::VR_Init(&initError, vr::VRApplication_Scene);
    vr::VRCompositor();

    m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);
    CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, leftEyeDesc);
    CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc);
    vr::VRInput()->SetActionManifestPath("C:/Users/Caleb/Documents/GitHub/McShooty/build/out/Assets/hellovr_actions.json");

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

    eyes[vr::EVREye::Eye_Left].projection = ConvertToGlmMatrix4(m_pHMD->GetProjectionMatrix(vr::EVREye::Eye_Left, .1f, 1000.0f));
    eyes[vr::EVREye::Eye_Left].view = glm::inverse((ConvertToGlmMatrix4(m_pHMD->GetEyeToHeadTransform(vr::EVREye::Eye_Left))));
    eyes[vr::EVREye::Eye_Right].projection = ConvertToGlmMatrix4(m_pHMD->GetProjectionMatrix(vr::EVREye::Eye_Right, .1f, 1000.0f));;
    eyes[vr::EVREye::Eye_Right].view = glm::inverse((ConvertToGlmMatrix4(m_pHMD->GetEyeToHeadTransform(vr::EVREye::Eye_Right))));
}

void SteamVrApi::UpdateDevices()
{
    vr::VREvent_t event;
    while (m_pHMD->PollNextEvent(&event, sizeof(event)))
    {
    }

    vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
    if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
    {
        m_mat4HMDPose = glm::inverse(ConvertToGlmMatrix4(m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking));
    }
    vr::VRActiveActionSet_t actionSet = { 0 };
    actionSet.ulActionSet = m_actionsetDemo;
    vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);
    vr::InputPoseActionData_t poseData;
    vr::EVRInputError inputErrorHand = vr::VRInput()->GetPoseActionDataForNextFrame(m_rHand[EHand::Left].m_actionPose, vr::TrackingUniverseStanding, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle);

    if (inputErrorHand == vr::EVRInputError::VRInputError_None)
    {
        m_rHand[EHand::Left].m_rmat4Pose = ConvertToGlmMatrix4(poseData.pose.mDeviceToAbsoluteTracking);
        
        trackedDeviceTransforms[ETrackedDeviceType::LeftHandController].worldMatrix = m_rHand[EHand::Left].m_rmat4Pose;
        TransformSystem::DecomposeTransform(trackedDeviceTransforms[ETrackedDeviceType::LeftHandController]);
    }

    inputErrorHand = vr::VRInput()->GetPoseActionDataForNextFrame(m_rHand[EHand::Right].m_actionPose, vr::TrackingUniverseStanding, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle);

    if (inputErrorHand == vr::EVRInputError::VRInputError_None)
    {
        m_rHand[EHand::Right].m_rmat4Pose = ConvertToGlmMatrix4(poseData.pose.mDeviceToAbsoluteTracking);

        trackedDeviceTransforms[ETrackedDeviceType::RightHandController].worldMatrix = m_rHand[EHand::Right].m_rmat4Pose;
        TransformSystem::DecomposeTransform(trackedDeviceTransforms[ETrackedDeviceType::RightHandController]);
    }


    eyes[vr::EVREye::Eye_Left].view = glm::inverse((ConvertToGlmMatrix4(m_pHMD->GetEyeToHeadTransform(vr::EVREye::Eye_Left)))) * m_mat4HMDPose;
    eyes[vr::EVREye::Eye_Left].projection = ConvertToGlmMatrix4(m_pHMD->GetProjectionMatrix(vr::EVREye::Eye_Left, .1f, 1000.f));
    eyes[vr::EVREye::Eye_Right].view = glm::inverse((ConvertToGlmMatrix4(m_pHMD->GetEyeToHeadTransform(vr::EVREye::Eye_Right)))) * m_mat4HMDPose;
    eyes[vr::EVREye::Eye_Right].projection = ConvertToGlmMatrix4(m_pHMD->GetProjectionMatrix(vr::EVREye::Eye_Right, .1f, 1000.f));
}

const Transform& SteamVrApi::GetTrackedDeviceTransform(ETrackedDeviceType type)
{
    return trackedDeviceTransforms[type];
}

void SteamVrApi::DrawFrame(std::vector<RenderedObject>& objects, OpenGLRenderSystem& renderSystem)
{
    renderSystem.LoadRenderCameraParams(eyes[vr::EVREye::Eye_Left]);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
    glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    for (auto& obj : objects)
    {
        renderSystem.Draw(obj);
    }

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

    renderSystem.LoadRenderCameraParams(eyes[vr::EVREye::Eye_Right]);

    glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
    glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
    //RenderScene(vr::Eye_Right);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    for (auto& obj : objects)
    {
        renderSystem.Draw(obj);
    }

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

}