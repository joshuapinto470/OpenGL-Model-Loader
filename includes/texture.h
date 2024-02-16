#pragma once
#include <string>

class Texture {
    public:
        Texture(const std::string&);
        void Bind();
    private:
        unsigned int textureID;
};