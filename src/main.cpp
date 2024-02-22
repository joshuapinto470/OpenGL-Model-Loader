#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <shader.h>
#include <texture.h>
#include <camera.h>
#include <model.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow *, int, int);
void mouse_callback(GLFWwindow *, double, double);
void processInput(GLFWwindow *);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float xoffset, yoffset;
bool firstMouse = true;
float lastX, lastY;

glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
Camera cam(pos, -90.0f, 0.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Model Loader", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader mShader("../shaders/vertex.vert", "../shaders/fragment.frag");

    Model scene("../resources/models/monkey.dae");

    Texture tex1("../resources/textures/container.jpg", "", "texture_diffuse");
    Texture tex2("../resources/textures/awesomeface.png", "", "texture_diffuse");
    glEnable(GL_DEPTH_TEST);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    mShader.use();
    mShader.setInt("texture1", 0);
    mShader.setInt("texture2", 1);
    mShader.setMat4("model", model);

    mShader.setMat4("projection", projection);

    const float radius = 10.0f;
    float lastFrame = 0.0f;
    float deltaTime;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        cam.UpdateCamera(window, deltaTime);
        glm::mat4 view(1.0f);
        view = cam.getCameraViewMatrix();

        glActiveTexture(GL_TEXTURE0);
        tex1.Bind();
        glActiveTexture(GL_TEXTURE1);
        tex2.Bind();

        mShader.use();
        mShader.setMat4("view", view);

        scene.Draw(mShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}



void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    xoffset = xpos - lastX;
    yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
Mesh processMesh(aiMesh *, const aiScene *);
void processNode(aiNode *, const aiScene *);

int main()
{
    Assimp::Importer import;
    const aiScene *pScene = import.ReadFile("../resources/models/basic_scene.dae", aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
    {
        std ::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return -1;
    }

    aiNode *rootNode = pScene->mRootNode;
    // std::cout << "Root node : " << rootNode->mChildren[0]->mMeshes[1] << std::endl;
    // std::cout << "Num Meshes : " << rootNode->mNumMeshes;
    processNode(rootNode, pScene);

    return 0;
}

void processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    std :: cout << "Loading mesh " << mesh->mName.C_Str() << std :: endl;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            std :: cout << "UV: " << vec.x << ", " << vec.y << ".\n"; 
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++){
            std :: cout << "f: " << face.mIndices[j];
            indices.push_back(face.mIndices[j]);
        }
        std :: cout << "end\n";
    }
    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    aiString str;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

    std :: cout << "Mat type : " <<  str.C_Str() << std::endl;

    // return a mesh object created from the extracted mesh data
    for (int i = 0; i < vertices.size(); i++)
    {
        std ::cout << "( " << vertices[i].Position.x;
        std ::cout << ", " << vertices[i].Position.y;
        std ::cout << ", " << vertices[i].Position.z << ")" << std::endl;
    }

    std ::cout << "Mesh End!" << std ::endl;
    return Mesh(vertices, indices, textures);
}
*/