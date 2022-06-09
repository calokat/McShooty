#include "OpenXrApi.h"
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <gl/glew.h>
#include "WindowsPlatform.h"
#include "OpenGLAPI.h"
OpenXrApi::OpenXrApi(IPlatform& plat, IGraphicsAPI& graph, PE::GraphicsAPI gApiType) : platformApi(plat), graphicsApi(graph), graphicsApiType(gApiType)
{

}

XrResult OpenXrApi::Init()
{
	XrResult res;
	uint32_t nApiLayerProps;
	res = xrEnumerateApiLayerProperties(0, &nApiLayerProps, nullptr);
	std::vector<XrApiLayerProperties> layerProps(nApiLayerProps);
	xrEnumerateApiLayerProperties(nApiLayerProps, &nApiLayerProps, layerProps.data());
	std::cout << "Number of Api Layer Properties: " << nApiLayerProps << std::endl;
	for (int i = 0; i < nApiLayerProps; ++i)
	{
		std::cout << layerProps[i].layerName << std::endl;
	}

	uint32_t nInstanceExtensionProps;
	xrEnumerateInstanceExtensionProperties(nullptr, 0, &nInstanceExtensionProps, nullptr);
	std::vector<XrExtensionProperties> extensionProps(nInstanceExtensionProps, { XR_TYPE_EXTENSION_PROPERTIES });
	xrEnumerateInstanceExtensionProperties(nullptr, nInstanceExtensionProps, &nInstanceExtensionProps, extensionProps.data());
	res = CreateXRInstance();
	return res;
}

XrResult OpenXrApi::CreateXRInstance()
{
	assert(m_instance == XR_NULL_HANDLE);
	std::vector<const char*> extensions;
	const std::vector<const char*> platformExtensions = std::vector<const char*>();
	std::transform(platformExtensions.begin(), platformExtensions.end(), std::back_inserter(extensions),
		[](const char* ext) { return ext; });
	const std::vector<const char*> graphicsExtensions = { "XR_KHR_opengl_enable" };
	std::transform(graphicsExtensions.begin(), graphicsExtensions.end(), std::back_inserter(extensions),
		[](const char* ext) { return ext; });


	XrInstanceCreateInfo createInfo = { XR_TYPE_INSTANCE_CREATE_INFO };
	createInfo.next = nullptr;
	createInfo.createFlags = 0;
	createInfo.enabledApiLayerCount = 0;
	createInfo.enabledApiLayerNames = nullptr;
	createInfo.enabledExtensionCount = (uint32_t)extensions.size();
	createInfo.enabledExtensionNames = extensions.data();
	strcpy_s(createInfo.applicationInfo.applicationName, "Portable Engine");
	createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
	XrResult createInstanceResult = xrCreateInstance(&createInfo, &m_instance);
	if (!XR_SUCCEEDED(createInstanceResult))
	{
		throw "There is no XR instance";
	}
	return createInstanceResult;
}

void OpenXrApi::InitializeXRSystem()
{
	XrSystemGetInfo systemInfo{ XR_TYPE_SYSTEM_GET_INFO };
	systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	m_environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	XrResult res = xrGetSystem(m_instance, &systemInfo, &m_systemId);
	//graphicsPlugin->InitializeDeviceForXR(m_instance, m_systemId);





	PFN_xrGetOpenGLGraphicsRequirementsKHR pfnGetOpenGLGraphicsRequirementsKHR = nullptr;
	xrGetInstanceProcAddr(instance, "xrGetOpenGLGraphicsRequirementsKHR",
		reinterpret_cast<PFN_xrVoidFunction*>(&pfnGetOpenGLGraphicsRequirementsKHR));

	XrGraphicsRequirementsOpenGLKHR graphicsRequirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR };
	pfnGetOpenGLGraphicsRequirementsKHR(instance, m_systemId, &graphicsRequirements);

	GLint major = 0;
	GLint minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	const XrVersion desiredApiVersion = XR_MAKE_VERSION(major, minor, 0);
	if (graphicsRequirements.minApiVersionSupported > desiredApiVersion) {
		printf("Runtime does not support desired Graphics API and/or version");
	}
	OpenGLAPI& glApi = dynamic_cast<OpenGLAPI&>(graphicsApi);
#ifdef _WIN32
	WindowsPlatform& winApi = dynamic_cast<WindowsPlatform&>(platformApi);
	//m_graphicsBinding.hDC = (HDC)window->deviceContext;
	//m_graphicsBinding.hGLRC = (HGLRC)glContext->GetContext(window);
	m_graphicsBinding.hDC = winApi.GetDeviceContext();
	WinOpenGLContext& winGlContext = dynamic_cast<WinOpenGLContext&>(glApi.GetOpenGLContext());
	m_graphicsBinding.hGLRC = winGlContext.GetContext();
#endif
	// this was originally in InitializeResources() from hello_xr, but I think I only need this part of it
	glGenFramebuffers(1, &m_swapchainFramebuffer);

}
