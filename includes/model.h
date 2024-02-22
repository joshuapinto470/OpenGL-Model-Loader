#pragma once

#include <texture.h>
#include <mesh.h>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const char *);
    void Draw(Shader&);

private:
    std::vector<Mesh> m_meshes;
    std::vector<Texture> m_textures_loaded;

    void loadModel(std::string);
    void processNode(aiNode *, const aiScene *);
    Mesh processMesh(aiMesh *, const aiScene *);
    // std::vector<Texture> loadMaterialTextures(aiMaterial *, aiTextureType , std::string );
};