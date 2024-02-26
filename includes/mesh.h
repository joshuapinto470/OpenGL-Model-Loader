#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <shader.h>
#include <texture.h>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    // glm::vec3 Bitangent;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex>, std::vector<unsigned int>, std::vector<unsigned>);
    void Draw(Shader &shader);

    std::vector<unsigned> mTextures;
private:
    void initMesh();
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned> m_Indices;
    glm::mat4x4 m_transform;
    unsigned int VBO, VAO, EBO;
};