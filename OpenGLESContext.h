#if defined(__EMSCRIPTEN__) || defined(__linux__)
#pragma once
//#include "esUtil.h"
#include "IOpenGLContext.h"
#include "IPlatform.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>

class OpenGLESContext : public IOpenGLContext
{
public:
	void* GetContext();
	void _SwapBuffers(int winHandle);
	OpenGLESContext(IPlatform* plat);
private:
	IPlatform* platform;
	// EGLDisplay eglDisplay;
	// EGLSurface eglSurface;
	SDL_GLContext g_GLContext = NULL;

};
#endif