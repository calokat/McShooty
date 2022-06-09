#pragma once
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
private:
    XrResult Init();
    XrResult CreateXRInstance();
    void InitializeXRSystem();
    XrInstance m_instance;
    XrEnvironmentBlendMode m_environmentBlendMode;
    XrSystemId m_systemId;
    XrInstance instance;
#ifdef _WIN32
    XrGraphicsBindingOpenGLWin32KHR m_graphicsBinding;
#endif
    IPlatform& platformApi;
    IGraphicsAPI& graphicsApi;
    PE::GraphicsAPI graphicsApiType;


    GLuint m_swapchainFramebuffer;
};