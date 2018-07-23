#pragma once
#include <string>
#include <assimp/types.h>

struct Texture
{
    uint32_t id;
    std::string type;
    aiString path;
};
