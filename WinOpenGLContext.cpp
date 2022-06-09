#ifdef _WIN64
#include "WinOpenGLContext.h"

HGLRC WinOpenGLContext::GetContext()
{
	if (m_hrc != NULL)
	{
		return m_hrc;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(platform.GetDeviceContext(), &pfd);

	SetPixelFormat(platform.GetDeviceContext(), nPixelFormat, &pfd);

	HGLRC tempContext = wglCreateContext(platform.GetDeviceContext());
	wglMakeCurrent(platform.GetDeviceContext(), tempContext);

	glewExperimental = true;
	GLenum  glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glewStatus));
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	m_hrc = wglCreateContextAttribsARB(platform.GetDeviceContext(), 0, attribs);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);
	wglMakeCurrent(platform.GetDeviceContext(), m_hrc);

	if (!m_hrc) throw "Unable to get a GL context";


	return m_hrc;
}

void WinOpenGLContext::_SwapBuffers()
{
	SwapBuffers(platform.GetDeviceContext());
}
WinOpenGLContext::WinOpenGLContext(WindowsPlatform& plat) : platform(plat)
{
	GetContext();
}
#endif