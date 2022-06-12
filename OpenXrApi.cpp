#include "OpenXrApi.h"
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <gl/glew.h>
#include "WindowsPlatform.h"
#include "OpenGLAPI.h"
#include "xr_linear.h"

OpenXrApi::OpenXrApi(IPlatform& plat, IGraphicsAPI& graph, PE::GraphicsAPI gApiType) : platformApi(plat), graphicsApi(graph), graphicsApiType(gApiType)
{
	Init();
	InitializeXRSystem();
	InitializeXRSession();
	InitializeActions();
	CreateSwapchains();
}

void OpenXrApi::Frame(std::vector<RenderedObject> objects, OpenGLRenderSystem& renderSystem, const Transform& camTransform)
{
	PollEvents();
	if (!m_sessionRunning) return;
	XrFrameState frameState = BeginFrame();
	LocateViews(frameState.predictedDisplayTime);
	PollActions();
	CalculateCameraViews(camTransform);
	RenderFrame(objects, renderSystem, camTransform, frameState);
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




	// TODO: Put below code in separate plugin
	PFN_xrGetOpenGLGraphicsRequirementsKHR pfnGetOpenGLGraphicsRequirementsKHR = nullptr;
	xrGetInstanceProcAddr(m_instance, "xrGetOpenGLGraphicsRequirementsKHR",
		reinterpret_cast<PFN_xrVoidFunction*>(&pfnGetOpenGLGraphicsRequirementsKHR));

	XrGraphicsRequirementsOpenGLKHR graphicsRequirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR };
	pfnGetOpenGLGraphicsRequirementsKHR(m_instance, m_systemId, &graphicsRequirements);

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
	m_graphicsBinding.hDC = winApi.GetDeviceContext();
	WinOpenGLContext& winGlContext = dynamic_cast<WinOpenGLContext&>(glApi.GetOpenGLContext());
	m_graphicsBinding.hGLRC = winGlContext.GetContext();
#endif
	// this was originally in InitializeResources() from hello_xr, but I think I only need this part of it
	glGenFramebuffers(1, &m_swapchainFramebuffer);

}

void OpenXrApi::InitializeXRSession()
{
	XrSessionCreateInfo createInfo{ XR_TYPE_SESSION_CREATE_INFO };
	createInfo.next = reinterpret_cast<const XrBaseInStructure*>(&m_graphicsBinding);
	createInfo.systemId = m_systemId;
	XrResult res = xrCreateSession(m_instance, &createInfo, &m_session);


	XrReferenceSpaceCreateInfo referenceSpaceCreateInfo{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
	referenceSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
	XrPosef t{};
	t.orientation.w = 1;
	referenceSpaceCreateInfo.poseInReferenceSpace = t;
	res = xrCreateReferenceSpace(m_session, &referenceSpaceCreateInfo, &m_appSpace);
}

void OpenXrApi::InitializeActions()
{
	// Create an action set.
	{
		XrActionSetCreateInfo actionSetInfo{ XR_TYPE_ACTION_SET_CREATE_INFO };
		strcpy_s(actionSetInfo.actionSetName, "gameplay");
		strcpy_s(actionSetInfo.localizedActionSetName, "Gameplay");
		actionSetInfo.priority = 0;
		xrCreateActionSet(m_instance, &actionSetInfo, &inputState.actionSet);
	}

	// Get the XrPath for the left and right hands - we will use them as subaction paths.
	xrStringToPath(m_instance, "/user/hand/left", &inputState.handSubactionPath[Side::LEFT]);
	xrStringToPath(m_instance, "/user/hand/right", &inputState.handSubactionPath[Side::RIGHT]);

	{
		XrActionCreateInfo actionInfo{ XR_TYPE_ACTION_CREATE_INFO };
		// Create an input action getting the left and right hand poses.
		actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
		strcpy_s(actionInfo.actionName, "hand_pose");
		strcpy_s(actionInfo.localizedActionName, "Hand Pose");
		actionInfo.countSubactionPaths = uint32_t(inputState.handSubactionPath.size());
		actionInfo.subactionPaths = inputState.handSubactionPath.data();
		xrCreateAction(inputState.actionSet, &actionInfo, &inputState.poseAction);
	}

	std::array<XrPath, 2> posePath;
	xrStringToPath(m_instance, "/user/hand/left/input/grip/pose", &posePath[Side::LEFT]);
	xrStringToPath(m_instance, "/user/hand/right/input/grip/pose", &posePath[Side::RIGHT]);

	{
		XrPath oculusTouchInteractionProfilePath;
		xrStringToPath(m_instance, "/interaction_profiles/oculus/touch_controller", &oculusTouchInteractionProfilePath);
		std::vector<XrActionSuggestedBinding> bindings{ {
														{inputState.poseAction, posePath[Side::LEFT]},
														{inputState.poseAction, posePath[Side::RIGHT]}
		} };
		XrInteractionProfileSuggestedBinding suggestedBindings{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
		suggestedBindings.interactionProfile = oculusTouchInteractionProfilePath;
		suggestedBindings.suggestedBindings = bindings.data();
		suggestedBindings.countSuggestedBindings = (uint32_t)bindings.size();
		xrSuggestInteractionProfileBindings(m_instance, &suggestedBindings);
	}

	// Suggest bindings for the Valve Index Controller.
	{
		XrPath indexControllerInteractionProfilePath;
		xrStringToPath(m_instance, "/interaction_profiles/valve/index_controller", &indexControllerInteractionProfilePath);
		//std::vector<XrActionSuggestedBinding> bindings{ {{m_input.grabAction, squeezeForcePath[Side::LEFT]},
		//												{m_input.grabAction, squeezeForcePath[Side::RIGHT]},
		//												{m_input.poseAction, posePath[Side::LEFT]},
		//												{m_input.poseAction, posePath[Side::RIGHT]},
		//												{m_input.quitAction, bClickPath[Side::LEFT]},
		//												{m_input.quitAction, bClickPath[Side::RIGHT]},
		//												{m_input.vibrateAction, hapticPath[Side::LEFT]},
		//												{m_input.vibrateAction, hapticPath[Side::RIGHT]}} };
		std::vector<XrActionSuggestedBinding> bindings{
			{inputState.poseAction, posePath[Side::LEFT]},
			{inputState.poseAction, posePath[Side::RIGHT]}
		};
		XrInteractionProfileSuggestedBinding suggestedBindings{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
		suggestedBindings.interactionProfile = indexControllerInteractionProfilePath;
		suggestedBindings.suggestedBindings = bindings.data();
		suggestedBindings.countSuggestedBindings = (uint32_t)bindings.size();
		xrSuggestInteractionProfileBindings(m_instance, &suggestedBindings);
	}

	XrActionSpaceCreateInfo actionSpaceInfo{ XR_TYPE_ACTION_SPACE_CREATE_INFO };
	actionSpaceInfo.action = inputState.poseAction;
	actionSpaceInfo.poseInActionSpace.orientation.w = 1.f;
	actionSpaceInfo.subactionPath = inputState.handSubactionPath[Side::LEFT];
	xrCreateActionSpace(m_session, &actionSpaceInfo, &inputState.handSpace[Side::LEFT]);
	actionSpaceInfo.subactionPath = inputState.handSubactionPath[Side::RIGHT];
	xrCreateActionSpace(m_session, &actionSpaceInfo, &inputState.handSpace[Side::RIGHT]);

	XrSessionActionSetsAttachInfo attachInfo{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
	attachInfo.countActionSets = 1;
	attachInfo.actionSets = &inputState.actionSet;
	xrAttachSessionActionSets(m_session, &attachInfo);
}

void OpenXrApi::CreateSwapchains()
{
	XrResult res;
	XrSystemProperties systemProperties{ XR_TYPE_SYSTEM_PROPERTIES };
	res = xrGetSystemProperties(m_instance, m_systemId, &systemProperties);
	uint32_t viewCount;
	res = xrEnumerateViewConfigurationViews(m_instance, m_systemId, m_viewConfigType, 0, &viewCount, nullptr);
	m_configViews.resize(viewCount, { XR_TYPE_VIEW_CONFIGURATION_VIEW });
	res = xrEnumerateViewConfigurationViews(m_instance, m_systemId, m_viewConfigType, viewCount, &viewCount,
		m_configViews.data());

	// Create and cache view buffer for xrLocateViews later.
	m_views.resize(viewCount, { XR_TYPE_VIEW });

	// Create the swapchain and get the images.
	if (viewCount > 0) {
		// Select a swapchain format.
		uint32_t swapchainFormatCount;
		res = xrEnumerateSwapchainFormats(m_session, 0, &swapchainFormatCount, nullptr);
		std::vector<int64_t> swapchainFormats(swapchainFormatCount);
		res = xrEnumerateSwapchainFormats(m_session, (uint32_t)swapchainFormats.size(), &swapchainFormatCount,
			swapchainFormats.data());
		//swapchainFormatCount == swapchainFormats.size();


		// TODO: Put below code in separate plugin
		constexpr int64_t SupportedColorSwapchainFormats[] = {
			GL_RGB10_A2,
			GL_RGBA16F,
			// The two below should only be used as a fallback, as they are linear color formats without enough bits for color
			// depth, thus leading to banding.
			GL_RGBA8,
			GL_RGBA8_SNORM,
		};

		auto swapchainFormatIt =
			std::find_first_of(swapchainFormats.begin(), swapchainFormats.end(), std::begin(SupportedColorSwapchainFormats),
				std::end(SupportedColorSwapchainFormats));
		if (swapchainFormatIt == swapchainFormats.end()) {
			printf("No runtime swapchain format supported for color swapchain");
		}
		m_colorSwapchainFormat = *swapchainFormatIt;
	}

	// Create a swapchain for each view.
	for (uint32_t i = 0; i < viewCount; i++) {
		const XrViewConfigurationView& vp = m_configViews[i];

		// Create the swapchain.
		XrSwapchainCreateInfo swapchainCreateInfo{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
		swapchainCreateInfo.arraySize = 1;
		swapchainCreateInfo.format = m_colorSwapchainFormat;
		swapchainCreateInfo.width = vp.recommendedImageRectWidth;
		swapchainCreateInfo.height = vp.recommendedImageRectHeight;
		swapchainCreateInfo.mipCount = 1;
		swapchainCreateInfo.faceCount = 1;
		// TODO: This method just returns a field in vp so I will do that directly for now. It might be a good idea to implement the method later though.
		//swapchainCreateInfo.sampleCount = m_graphicsPlugin->GetSupportedSwapchainSampleCount(vp);
		swapchainCreateInfo.sampleCount = vp.recommendedSwapchainSampleCount;
		swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
		Swapchain swapchain;
		swapchain.width = swapchainCreateInfo.width;
		swapchain.height = swapchainCreateInfo.height;
		res = xrCreateSwapchain(m_session, &swapchainCreateInfo, &swapchain.handle);

		m_swapchains.push_back(swapchain);

		uint32_t imageCount;
		res = xrEnumerateSwapchainImages(swapchain.handle, 0, &imageCount, nullptr);
		
		// TODO: Put below code in separate graphics plugin (formerly in graphicsPlugin->AllocateSwapchainImageStructs()



		std::vector<XrSwapchainImageOpenGLKHR> swapchainImageBuffer(imageCount);
		std::vector<XrSwapchainImageBaseHeader*> swapchainImageBase;
		for (XrSwapchainImageOpenGLKHR& image : swapchainImageBuffer) {
			image.type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR;
			swapchainImageBase.push_back(reinterpret_cast<XrSwapchainImageBaseHeader*>(&image));
		}

		// Keep the buffer alive by moving it into the list of buffers.
		m_swapchainImageBuffers.push_back(std::move(swapchainImageBuffer));

		std::vector<XrSwapchainImageBaseHeader*> swapchainImages = swapchainImageBase;
		res = xrEnumerateSwapchainImages(swapchain.handle, imageCount, &imageCount, swapchainImages[0]);

		m_swapchainImages.insert(std::make_pair(swapchain.handle, std::move(swapchainImages)));
	}
}

XrFrameState OpenXrApi::BeginFrame()
{
	assert(m_session != XR_NULL_HANDLE);

	XrFrameWaitInfo frameWaitInfo{ XR_TYPE_FRAME_WAIT_INFO };
	XrFrameState frameState{ XR_TYPE_FRAME_STATE };
	assert(XR_SUCCEEDED(xrWaitFrame(m_session, &frameWaitInfo, &frameState)));

	XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO };
	assert(XR_SUCCEEDED(xrBeginFrame(m_session, &frameBeginInfo)));

	return frameState;
}

void OpenXrApi::PollActions()
{
	// Sync actions
	const XrActiveActionSet activeActionSet{ inputState.actionSet, XR_NULL_PATH };
	XrActionsSyncInfo syncInfo{ XR_TYPE_ACTIONS_SYNC_INFO };
	syncInfo.countActiveActionSets = 1;
	syncInfo.activeActionSets = &activeActionSet;
	XrResult res = xrSyncActions(m_session, &syncInfo);
}

const XrEventDataBaseHeader* OpenXrApi::TryReadNextEvent()
{
	// It is sufficient to clear the just the XrEventDataBuffer header to
	// XR_TYPE_EVENT_DATA_BUFFER
	XrEventDataBaseHeader* baseHeader = reinterpret_cast<XrEventDataBaseHeader*>(&m_eventDataBuffer);
	*baseHeader = { XR_TYPE_EVENT_DATA_BUFFER };
	const XrResult xr = xrPollEvent(m_instance, &m_eventDataBuffer);
	if (xr == XR_SUCCESS) {
		return baseHeader;
	}
	if (xr == XR_EVENT_UNAVAILABLE) {
		return nullptr;
	}
	throw "Bad poll event";
}

void OpenXrApi::PollEvents()
{
	while (const XrEventDataBaseHeader* event = TryReadNextEvent()) {
		switch (event->type) {
			case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
				const auto& instanceLossPending = *reinterpret_cast<const XrEventDataInstanceLossPending*>(event);
				return;
			}
			case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
				auto sessionStateChangedEvent = *reinterpret_cast<const XrEventDataSessionStateChanged*>(event);
				HandleSessionStateChangedEvent(sessionStateChangedEvent/*, exitRenderLoop, requestRestart*/);
				break;
			}
		}
	}
}

void OpenXrApi::HandleSessionStateChangedEvent(const XrEventDataSessionStateChanged& stateChangedEvent)
{
	const XrSessionState oldState = m_sessionState;
	m_sessionState = stateChangedEvent.state;
	switch (m_sessionState) {
	case XR_SESSION_STATE_READY: {
		assert(m_session != XR_NULL_HANDLE);
		XrSessionBeginInfo sessionBeginInfo{ XR_TYPE_SESSION_BEGIN_INFO };
		sessionBeginInfo.primaryViewConfigurationType = m_viewConfigType;
		assert(XR_SUCCEEDED(xrBeginSession(m_session, &sessionBeginInfo)));
		m_sessionRunning = true;
		break;
	}
	case XR_SESSION_STATE_STOPPING: {
		assert(m_session != XR_NULL_HANDLE);
		m_sessionRunning = false;
		assert(XR_SUCCEEDED(xrEndSession(m_session)));
		break;
	}
	default:
		break;
	}
}

bool OpenXrApi::LocateViews(XrTime predictedDisplayTime)
{
	XrResult res;

	XrViewState viewState{ XR_TYPE_VIEW_STATE };
	uint32_t viewCapacityInput = (uint32_t)m_views.size();
	uint32_t viewCountOutput;

	XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO };
	viewLocateInfo.viewConfigurationType = m_viewConfigType;
	viewLocateInfo.displayTime = predictedDisplayTime;
	viewLocateInfo.space = m_appSpace;

	res = xrLocateViews(m_session, &viewLocateInfo, &viewState, viewCapacityInput, &viewCountOutput, m_views.data());
	assert(XR_SUCCEEDED(res));
	assert(viewCountOutput == viewCapacityInput);
	assert(viewCountOutput == m_configViews.size());
	assert(viewCountOutput == m_swapchains.size());

	if ((viewState.viewStateFlags & XR_VIEW_STATE_POSITION_VALID_BIT) == 0 ||
		(viewState.viewStateFlags & XR_VIEW_STATE_ORIENTATION_VALID_BIT) == 0) {
		return false;  // There is no valid tracking poses for the views.
	}
	return true;

}

void OpenXrApi::CalculateCameraViews(const Transform& primaryCamTransform)
{
	viewCams.resize(m_views.size());
	for (int i = 0; i < m_views.size(); ++i)
	{
		const auto& pose = m_views[i].pose;
		XrMatrix4x4f proj;
		XrMatrix4x4f_CreateProjectionFov(&proj, graphicsApiEnum, m_views[i].fov, 0.05f, 100.0f);
		XrMatrix4x4f toView;
		XrVector3f scale{ 1.f, 1.f, 1.f };
		XrVector3f camOriginalPos = { primaryCamTransform.position.x, primaryCamTransform.position.y, primaryCamTransform.position.z };
		XrVector3f finalPos;
		XrVector3f_Add(&finalPos, &camOriginalPos, &pose.position);
		XrMatrix4x4f_CreateTranslationRotationScale(&toView, &finalPos, &pose.orientation, &scale);
		XrMatrix4x4f view;
		XrMatrix4x4f_InvertRigidBody(&view, &toView);

		Camera toAdd;
		toAdd.view = glm::make_mat4(view.m);
		toAdd.projection = glm::make_mat4(proj.m);

		viewCams[i] = toAdd;
	}
}

void OpenXrApi::RenderFrame(std::vector<RenderedObject> objects, OpenGLRenderSystem& renderSystem, const Transform& camTransform, XrFrameState frameState)
{
	std::vector<XrCompositionLayerBaseHeader*> layers;
	XrCompositionLayerProjection layer{ XR_TYPE_COMPOSITION_LAYER_PROJECTION };
	std::vector<XrCompositionLayerProjectionView> projectionLayerViews;
	if (frameState.shouldRender == XR_TRUE) {
		if (RenderLayer(frameState.predictedDisplayTime, projectionLayerViews, layer, objects, renderSystem)) {
			layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&layer));
		}
	}

	XrFrameEndInfo frameEndInfo{ XR_TYPE_FRAME_END_INFO };
	frameEndInfo.displayTime = frameState.predictedDisplayTime;
	frameEndInfo.environmentBlendMode = m_environmentBlendMode;
	frameEndInfo.layerCount = (uint32_t)layers.size();
	frameEndInfo.layers = layers.data();
	assert(XR_SUCCEEDED(xrEndFrame(m_session, &frameEndInfo)));

}

uint32_t OpenXrApi::GetDepthTexture(uint32_t colorTexture)
{
	auto depthBufferIt = m_colorToDepthMap.find(colorTexture);
	if (depthBufferIt != m_colorToDepthMap.end()) {
		return depthBufferIt->second;
	}

	// This back-buffer has no corresponding depth-stencil texture, so create one with matching dimensions.
	GLint width;
	GLint height;
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	uint32_t depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	m_colorToDepthMap.insert(std::make_pair(colorTexture, depthTexture));
	return depthTexture;
}

bool OpenXrApi::RenderLayer(XrTime predictedDisplayTime, std::vector<XrCompositionLayerProjectionView>& projectionLayerViews, XrCompositionLayerProjection& layer, std::vector<RenderedObject> objects, OpenGLRenderSystem& renderSystem)
{
	projectionLayerViews.resize(m_views.size());
	// Render view to the appropriate part of the swapchain image.
	for (uint32_t i = 0; i < m_views.size(); i++) {
		// Each view has a separate swapchain which is acquired, rendered to, and released.
		renderSystem.LoadRenderCameraParams(viewCams[i]);
		const Swapchain viewSwapchain = m_swapchains[i];

		XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };

		uint32_t swapchainImageIndex;
		assert(XR_SUCCEEDED(xrAcquireSwapchainImage(viewSwapchain.handle, &acquireInfo, &swapchainImageIndex)));

		XrSwapchainImageWaitInfo waitInfo{ XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
		waitInfo.timeout = XR_INFINITE_DURATION;
		assert(XR_SUCCEEDED(xrWaitSwapchainImage(viewSwapchain.handle, &waitInfo)));

		projectionLayerViews[i] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
		projectionLayerViews[i].pose = m_views[i].pose;
		projectionLayerViews[i].fov = m_views[i].fov;
		projectionLayerViews[i].subImage.swapchain = viewSwapchain.handle;
		projectionLayerViews[i].subImage.imageRect.offset = { 0, 0 };
		projectionLayerViews[i].subImage.imageRect.extent = { viewSwapchain.width, viewSwapchain.height };

		const XrSwapchainImageBaseHeader* const swapchainImage = m_swapchainImages[viewSwapchain.handle][swapchainImageIndex];
		
		// TODO: Put following code in graphics plugin	
	
		//graphicsPlugin->RenderView(projectionLayerViews[i], swapchainImage, m_colorSwapchainFormat, reg, renderSystem, viewCams[i]);
		assert(projectionLayerViews[i].subImage.imageArrayIndex == 0);  // Texture arrays not supported.
		glBindFramebuffer(GL_FRAMEBUFFER, m_swapchainFramebuffer);
		const uint32_t colorTexture = reinterpret_cast<const XrSwapchainImageOpenGLKHR*>(swapchainImage)->image;
		glViewport(static_cast<GLint>(projectionLayerViews[i].subImage.imageRect.offset.x),
			static_cast<GLint>(projectionLayerViews[i].subImage.imageRect.offset.y),
			static_cast<GLsizei>(projectionLayerViews[i].subImage.imageRect.extent.width),
			static_cast<GLsizei>(projectionLayerViews[i].subImage.imageRect.extent.height));
		glEnable(GL_DEPTH_TEST);

		const uint32_t depthTexture = GetDepthTexture(colorTexture);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		// Clear swapchain and depth buffer.
		//glClearColor(DarkSlateGray[0], DarkSlateGray[1], DarkSlateGray[2], DarkSlateGray[3]);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		for (auto& ro : objects)
		{
			renderSystem.Draw(ro);
		}


		// end TODO
		XrSwapchainImageReleaseInfo releaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
		assert(XR_SUCCEEDED(xrReleaseSwapchainImage(viewSwapchain.handle, &releaseInfo)));
	}

	layer.space = m_appSpace;
	layer.viewCount = (uint32_t)projectionLayerViews.size();
	layer.views = projectionLayerViews.data();
	return true;
}
