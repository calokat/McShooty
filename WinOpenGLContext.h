#ifdef _WIN64
#pragma once
#include "IOpenGLContext.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include "WindowsPlatform.h"
class WinOpenGLContext :
	public IOpenGLContext
{
public:
	HGLRC GetContext();
	void _SwapBuffers();
	WinOpenGLContext(WindowsPlatform& plat);
private:
	HGLRC m_hrc = NULL;
	WindowsPlatform& platform;
};
#endif
