#pragma once
#include <string>

// class Texture
// {
// public:
//     Texture(const std::string &, const std::string &path, const std::string &type);
//     void Bind();
//     unsigned int textureID;
//     std::string path, type;
// };

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

unsigned int TextureFromFile(const char *, const std::string &, bool gamma = true);