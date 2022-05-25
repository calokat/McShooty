#include "OpenGLAPI.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui_impl_opengl3.h>

int OpenGLAPI::Init()
{
	glContext->GetContext();
	glEnable(GL_DEPTH_TEST);
	return 0;
}

void OpenGLAPI::ClearScreen()
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // rgb(33,150,243)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLAPI::_SwapBuffers()
{
	glContext->_SwapBuffers(-1);
}

void OpenGLAPI::NewGuiFrame()
{
}

void OpenGLAPI::DrawGui()
{
}

OpenGLAPI::OpenGLAPI(IPlatform* plat) : platform(plat)
{
#ifdef _WIN64
	glContext = new WinOpenGLContext(platform);
#elif defined(__EMSCRIPTEN__)
	glContext = new OpenGLESContext(platform);
#elif defined(__linux__)
	glContext = new OpenGLESContext(platform);
#endif
}

OpenGLAPI::~OpenGLAPI()
{
	delete glContext;
}
#ifndef __EMSCRIPTEN__
#endif
void OpenGLAPI::BindToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800, 600);
}

IOpenGLContext* OpenGLAPI::GetOpenGLContext()
{
	return glContext;
}
