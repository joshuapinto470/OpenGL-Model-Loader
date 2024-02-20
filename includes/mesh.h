#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <shader.h>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 TexCoords;
};

struct Texture
{
    unsigned int ID;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex>, std::vector<unsigned int>, std::vector<Texture>);
    void Draw(Shader& shader);

private:
    void initMesh();
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<Texture> mTextures;
    unsigned int VBO, VAO, EBO;
};