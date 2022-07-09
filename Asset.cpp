#include "Asset.h"

Asset::Asset(std::vector<char> b) : bytes(b) {}

std::vector<char> Asset::GetBytes()
{
    return this->bytes;
}