//
// Created by caleb on 11/12/2020.
//

#include "OpenGLAndroidContext.h"

OpenGLAndroidContext::OpenGLAndroidContext(EGLDisplay display, EGLSurface surface) {
    eglDisplay = display;
    eglSurface = surface;
}

void OpenGLAndroidContext::_SwapBuffers() {
    eglSwapBuffers(eglDisplay, eglSurface);
}