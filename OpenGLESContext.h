#if defined(__EMSCRIPTEN__) || defined(__linux__)
#pragma once
//#include "esUtil.h"
#include "IOpenGLContext.h"
#include "IPlatform.h"
#include <SDL/SDL.h>
#include <GL/glew.h>

class OpenGLESContext : public IOpenGLContext
{
public:
	void* GetContext();
	void _SwapBuffers();
	OpenGLESContext(SDL_Window* w);
private:
	// EGLDisplay eglDisplay;
	// EGLSurface eglSurface;
	SDL_Window* window;
	SDL_GLContext g_GLContext = NULL;

};
#endif