#if defined(__EMSCRIPTEN__) || defined(__linux__)
#include "OpenGLESContext.h"
#include <iostream>


	// ESContext esContext;

void* OpenGLESContext::GetContext()
{
	//GLuint flags = ES_WINDOW_RGB | ES_WINDOW_DEPTH;

	//EGLint attribList[] =
	//{
	//	EGL_RED_SIZE,       5,
	//	EGL_GREEN_SIZE,     6,
	//	EGL_BLUE_SIZE,      5,
	//	EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
	//	EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
	//	EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
	//	EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
	//	EGL_NONE
	//};


	//EGLint numConfigs;
	//EGLint majorVersion;
	//EGLint minorVersion;
	//EGLDisplay display;
	//EGLContext context;
	//EGLSurface surface;
	//EGLConfig config;
	//EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE, EGL_NONE };

	//// Get Display
	//display = eglGetDisplay((EGLNativeDisplayType)platform->GetDeviceContext());
	//if (display == EGL_NO_DISPLAY)
	//{
	//	return EGL_FALSE;
	//}

	//if (!eglInitialize(display, &majorVersion, &minorVersion))
	//{
	//	return EGL_FALSE;
	//}

	//if (!eglGetConfigs(display, NULL, 0, &numConfigs))
	//{
	//	return EGL_FALSE;
	//}

	//if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
	//{
	//	return EGL_FALSE;
	//}

	//surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)platform->GetWindowHandle(), NULL);

	//if (surface == EGL_NO_SURFACE)
	//{
	//	return EGL_FALSE;
	//}

	//context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	//if (context == EGL_NO_CONTEXT)
	//{
	//	return EGL_FALSE;
	//}

	//if (!eglMakeCurrent(display, surface, surface, context))
	//{
	//	return EGL_FALSE;
	//}


	//
	//glEnable(GL_DEPTH_TEST);
	// EmscriptenWebGLContextAttributes attr;
	//emscripten_webgl_init_context_attributes(&attr);
	// SDL_GL_SetAttribute(GL_CONTEXT_FLAGS, 0);
	// SDL_GL_SetAttribute(GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);


	g_GLContext = SDL_GL_CreateContext((SDL_Window*)platform->GetWindowHandle());
	glewInit();
	if (!g_GLContext)
	{
		fprintf(stderr, "Failed to initialize WebGL context!\n");
		return nullptr;
	}
	SDL_GL_SetSwapInterval(1);
	glEnable(GL_DEPTH_TEST);

	return g_GLContext;
}

void OpenGLESContext::_SwapBuffers(int winHandle)
{
	//eglSwapBuffers(eglDisplay, eglSurface);
	SDL_GL_MakeCurrent((SDL_Window*)platform->GetWindowHandle(), g_GLContext);
	SDL_GL_SwapWindow((SDL_Window*)platform->GetWindowHandle());
}
OpenGLESContext::OpenGLESContext(IPlatform* plat) : platform(plat)
{
}
#endif