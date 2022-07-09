#pragma once
#include "Asset.h"
#include <string>
class IAssetManager
{
public:
    virtual Asset LoadAsset(std::string path) = 0;
};