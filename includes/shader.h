#pragma once

class Shader
{
public:
    Shader(const char*, const char*);
    void use();

private:
    unsigned int ID;
};