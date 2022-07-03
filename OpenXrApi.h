#pragma once
#ifndef __EMSCRIPTEN__
#include "openxr/openxr.h"
#include <array>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "openxr/openxr_platform.h"
#include "options.h"
#include "IPlatform.h"
#include "IGraphicsAPI.h"
#include <gl/glew.h>
#include <vector>
#include <list>
#include <map>
#include "OpenGLRenderSystem.h"
#include "RenderedObject.h"
#include "Transform.h"
#include "xr_linear.h"

struct Swapchain {
    XrSwapchain handle;
    int32_t width;
    int32_t height;
};

enum Side {
    LEFT,
    RIGHT
};

struct InputState {
    XrActionSet actionSet{ XR_NULL_HANDLE };
    XrAction grabAction{ XR_NULL_HANDLE };
    XrAction poseAction{ XR_NULL_HANDLE };
    XrAction vibrateAction{ XR_NULL_HANDLE };
    XrAction quitAction{ XR_NULL_HANDLE };
    std::array<XrPath, 2> handSubactionPath;
    std::array<XrSpace, 2> handSpace;
    std::array<float, 2> handScale = { {1.0f, 1.0f} };
    std::array<XrBool32, 2> handActive;
};

class OpenXrApi
{
public:
    OpenXrApi(IPlatform& plat, IGraphicsAPI& graph, PE::GraphicsAPI gApiType);
    void Frame(std::vector<RenderedObject> objects, OpenGLRenderSystem& renderSystem, const Transform& camTransform);
private:
    XrResult Init();
    XrResult CreateXRInstance();
    void InitializeXRSystem();
    void InitializeXRSession();
    void InitializeActions();
    void CreateSwapchains();
    XrFrameState BeginFrame();
    void PollActions();
    const XrEventDataBaseHeader* TryReadNextEvent();
    void PollEvents();
    void HandleSessionStateChangedEvent(const XrEventDataSessionStateChanged& stateChangedEvent);
    //void UpdateDevices(XrTime predictedTime);
    bool LocateViews(XrTime predictedDisplayTime);
    void CalculateCameraViews(const Transform& primaryCamTransform);
    void RenderFrame(std::vector<RenderedObject> objects, OpenGLRenderSystem& renderSystem, const Transform& camTransform, XrFrameState frameState);
    uint32_t GetDepthTexture(uint32_t colorTexture);
    bool RenderLayer(XrTime predictedDisplayTime, std::vector<XrCompositionLayerProjectionView>& projectionLayerViews, XrCompositionLayerProjection& layer, std::vector<RenderedObject> objects, OpenGLRenderSystem& renderSystem);
    XrInstance m_instance{ XR_NULL_HANDLE };
    XrEnvironmentBlendMode m_environmentBlendMode;
    XrSystemId m_systemId;
    XrSessionState m_sessionState;
    XrSession m_session;
    XrSpace m_appSpace;
    XrViewConfigurationType m_viewConfigType{ XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO };
    InputState inputState;
    std::vector<Camera> viewCams;
    std::vector<XrViewConfigurationView> m_configViews;
    std::vector<Swapchain> m_swapchains;
    std::map<XrSwapchain, std::vector<XrSwapchainImageBaseHeader*>> m_swapchainImages;
    std::vector<XrView> m_views;
    std::list<std::vector<XrSwapchainImageOpenGLKHR>> m_swapchainImageBuffers;
    int64_t m_colorSwapchainFormat{ -1 };
    XrEventDataBuffer m_eventDataBuffer;
#ifdef _WIN32
    XrGraphicsBindingOpenGLWin32KHR m_graphicsBinding{ XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR };
#endif
    IPlatform& platformApi;
    IGraphicsAPI& graphicsApi;
    PE::GraphicsAPI graphicsApiType;
    bool m_sessionRunning = false;
    GraphicsAPI graphicsApiEnum;
    GLuint m_swapchainFramebuffer;
    std::map<uint32_t, uint32_t> m_colorToDepthMap;
};
#endif