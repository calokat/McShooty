#include "OpenGLAPI.h"

int OpenGLAPI::Init()
{
	glEnable(GL_DEPTH_TEST);
	return 0;
}

void OpenGLAPI::ClearScreen()
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // rgb(33,150,243)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

OpenGLAPI::OpenGLAPI(IOpenGLContext& ctx) : glContext(ctx)
{
}

void OpenGLAPI::_SwapBuffers()
{
	glContext._SwapBuffers();
}

void OpenGLAPI::NewGuiFrame()
{
}

void OpenGLAPI::DrawGui()
{
}

OpenGLAPI::~OpenGLAPI()
{
}
#ifndef __EMSCRIPTEN__
#endif
void OpenGLAPI::BindToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800, 600);
}

IOpenGLContext& OpenGLAPI::GetOpenGLContext()
{
	return glContext;
}
