#ifdef _WIN64
#pragma once
#include "IOpenGLContext.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include "IPlatform.h"
class WinOpenGLContext :
	public IOpenGLContext
{
public:
	void* GetContext();
	void _SwapBuffers(int windowHandle);
	WinOpenGLContext(IPlatform* plat);
private:
	HGLRC m_hrc;
	IPlatform* platform;
};
#endif
