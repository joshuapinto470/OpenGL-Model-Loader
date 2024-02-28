#pragma once

#include <texture.h>
#include <mesh.h>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>

class Model {
   public:
    Model(const char *);
    Model(const std::string);
    void Draw(Shader &);

   private:
    std::vector<Mesh> m_meshes;
    std::vector<Texture> m_textures;
    std::string m_directory;

    unsigned int m_total_vertices;
    unsigned int m_total_indices;

    void loadModel(std::string);
    void processNode(aiNode *, const aiScene *);
    Mesh processMesh(aiMesh *, const aiScene *);
    std::vector<unsigned> loadMaterialTextures(aiMaterial *, aiTextureType, std::string);
};