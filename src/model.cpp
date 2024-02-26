#include <model.h>
#include <iostream>

Model::Model(const char *path)
{
    loadModel(path);
}

void Model::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    // std:: cout << "size : " <<  m_textures.size();
    for (unsigned int j = 0; j < m_textures.size(); j++)
    {
        // unsigned index = m_meshes[i].mTextures[j];
        Texture tex = m_textures[j];

        glActiveTexture(GL_TEXTURE0 + j); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = tex.type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), j);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, tex.id);
    }

    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        // Get textures, shader to be bound and bind them.
        // std::cout << "DRRAW\n";

        // Draw the model.
        m_meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
                                                     aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    // const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std ::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    std::cout << "Model dir: " << m_directory << std::endl;
    m_total_vertices = 0;
    m_total_indices = 0;
    processNode(scene->mRootNode, scene);

    std ::cout << "Total meshes loaded: " << m_meshes.size() << std::endl;
    std ::cout << "Total vertices : " << m_total_vertices << "\n";
    std ::cout << "Total indices : " << m_total_indices << "\n";
    std ::cout << "Total textures: " << m_textures.size() << std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    aiMatrix4x4 transform = node->mTransformation;
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned> textures;

    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    std ::cout << "Processing mesh : " << mesh->mName.C_Str() << std ::endl;

    // Load mesh vertex data
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        // convert from blender to OpenGL coords
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // Load mesh index data
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Load mesh material data

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<unsigned> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<unsigned> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<unsigned> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<unsigned> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    std::cout << "Mesh size " << vertices.size() << ", " << indices.size() << ", " << textures.size() << "\n";
    // std::cout << "Vertices size : " << vertices.size() << "\n";
    // std::cout << "Indices size : " << indices.size() << "\n";
    m_total_vertices += vertices.size();
    m_total_indices += indices.size();
    return Mesh(vertices, indices, textures);
}

// Store the texture data in our model class and texture index in the mesh class
std::vector<unsigned> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<unsigned> textures;
    unsigned matTexCount = mat->GetTextureCount(type);

    // If no texture exists make a default texture
    bool skip = false;
    if (matTexCount == 0)
    {
        aiColor3D defaultColor;
        aiString mat_name;
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            mat->Get(AI_MATKEY_NAME, mat_name);
            mat->Get(AI_MATKEY_COLOR_DIFFUSE, defaultColor);
            std ::cout << "Attempting to create texture.\n"
                       << mat_name.C_Str() << "\n";
            for (unsigned i = 0; i < m_textures.size(); i++)
            {
                if (std::strcmp(m_textures[i].path.data(), mat_name.C_Str()) == 0)
                {
                    textures.push_back(i);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                Texture texture;
                unsigned char r = defaultColor.r * 255;
                unsigned char g = defaultColor.g * 255;
                unsigned char b = defaultColor.b * 255;

                texture.id = TextureFromData(r, g, b);
                texture.type = typeName;
                texture.path = mat_name.C_Str();
                m_textures.push_back(texture);
                textures.push_back(m_textures.size() - 1);
            }
            break;

        default:
            break;
        }

        return textures;
    }

    for (unsigned int i = 0; i < matTexCount; i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < m_textures.size(); j++)
        {
            if (std::strcmp(m_textures[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(j);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one.
                break;
            }
        }

        if (!skip)
        { // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            m_textures.push_back(texture); // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            textures.push_back(m_textures.size() - 1);
        }
    }
    return textures;
}