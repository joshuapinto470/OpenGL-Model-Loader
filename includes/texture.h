#pragma once
#include <string>

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

unsigned TextureFromData(char r, char g, char b);
unsigned int TextureFromFile(const char *, const std::string &, bool gamma = true);