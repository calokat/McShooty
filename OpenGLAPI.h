#pragma once
#include "IGraphicsAPI.h"
#ifdef _WIN64
#include "WinOpenGLContext.h"
#elif defined(__EMSCRIPTEN__) || defined(__linux__)
#include "OpenGLESContext.h"
#endif
#include "IPlatform.h"

class OpenGLAPI : public IGraphicsAPI
{
public:
	int Init();
	void ClearScreen();
	OpenGLAPI(IOpenGLContext& ctx);
	void _SwapBuffers();
	void NewGuiFrame();
	void DrawGui();
	~OpenGLAPI();
	void BindToScreen();
	// unique to OpenGLAPI, and not shared with the IGraphicsAPI interface
	IOpenGLContext& GetOpenGLContext();
private:
	IOpenGLContext& glContext;
};

