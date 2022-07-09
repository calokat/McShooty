#ifdef __ANDROID__
#pragma once
#include "IAssetManager.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class AndroidAssetManager: public IAssetManager
{
public:
    AndroidAssetManager(AAssetManager* mngr);
    Asset LoadAsset(std::string path);
private:
    AAssetManager* manager;
};
#endif