#ifdef __ANDROID__
#include "AndroidAssetManager.h"

AndroidAssetManager::AndroidAssetManager(AAssetManager* mngr): manager(mngr) {}

AndroidAssetManager::LoadAsset(std::string path)
{
    AAsset* androidAsset = AAssetManager_open(mngr, path, AASSET_MODE_UNKNOWN);
    std::vector<char> byteBuffer(AAsset_getLength(androidAsset));
    AAsset_read(androidAsset, byteBuffer.data(), byteBuffer.size());
    return Asset(byteBuffer);
}
#endif