#pragma once
#include <string>

class Texture {
    public:
        Texture(const std::string&, const std::string& path, const std::string& type);
        void Bind();
        unsigned int textureID;
        std::string path, type;
};