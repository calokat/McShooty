#ifdef _WIN32
#include "WindowsAssetManager.h"
#include <fstream>

Asset WindowsAssetManager::LoadAsset(std::string path)
{
    std::ifstream assetStream(path, std::ios::binary);
    assetStream.seekg(0, std::ios::end);
    std::vector<char> assetBuffer((int)assetStream.tellg() + 1);
    assetBuffer[assetBuffer.size() - 1] = '\0';
    assetStream.seekg(0, std::ios::beg);
    assetStream.read(assetBuffer.data(), assetBuffer.size());
    return Asset(assetBuffer);
}
#endif