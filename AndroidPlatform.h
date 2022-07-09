#pragma once
#ifndef IPLATFORM
#define IPLATFORM
#include "IPlatform.h"
#endif
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <EGL/egl.h>
#include <memory>
class AndroidPlatform : public IPlatform {
public:
    AndroidPlatform(ANativeWindow* win, AAssetManager* mgr);
    int InitWindow();
    bool Run();
    ~AndroidPlatform();
    EGLDisplay GetDisplay();
    EGLSurface GetSurface();
private:
    EGLDisplay display;
    EGLSurface surface;
    ANativeWindow* window;
    AAssetManager* manager;
    EGLContext context;
};