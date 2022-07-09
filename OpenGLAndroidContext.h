#pragma once
#include "IOpenGLContext.h"
#ifndef IPLATFORM
#define IPLATFORM
#include "IPlatform.h"
#endif
#include <EGL/egl.h>
#include <GLES/gl.h>

class OpenGLAndroidContext : public IOpenGLContext{
public:
    void _SwapBuffers();
    OpenGLAndroidContext(EGLDisplay display, EGLSurface surface);
private:
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;

};