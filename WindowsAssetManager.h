#pragma once
#ifdef _WIN32
#include "IAssetManager.h"

class WindowsAssetManager: public IAssetManager
{
public:
    Asset LoadAsset(std::string path);
};
#endif