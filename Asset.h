#pragma once
#include <vector>
class Asset
{
public:
    Asset(std::vector<char> b);
    std::vector<char> GetBytes();
private:
    std::vector<char> bytes;
};