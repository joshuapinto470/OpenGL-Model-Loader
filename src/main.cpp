#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

// #define TEST_MAIN

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
bool mouseEnable = true;
float lastX, lastY;

glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
Camera cam(pos, -90.0f, 0.0f);

#ifdef TEST_MAIN
#include <renderer.h>
void processNode(aiNode *, const aiScene *);
void processMesh(aiMesh *, const aiScene *);
int main(int argc, char const *argv[])
{
    // OGML::Render mRender(1200, 800);
    // mRender.Draw();
    // const std::string path = "../resources/models/backpack/backpack.obj";
    const std::string path = "../resources/models/basic_scene_textured.dae";

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate |
                                                       aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std ::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return -1;
    }

    std::string m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    return 0;
}


void processNode(aiNode *node, const aiScene *scene)
{
    aiMatrix4x4 node_transform = node->mTransformation;
    
    std::vector<Mesh> m_meshes;
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    std ::cout << "Processing mesh : " << mesh->mName.C_Str() << std ::endl;

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
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    aiString mat_name;
    aiColor3D color(0.0, 0.0, 0.0);
    aiColor3D reflective;
    aiColor3D specular;
    float roughness;

    material->Get(AI_MATKEY_NAME, mat_name);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material->Get(AI_MATKEY_COLOR_REFLECTIVE, reflective);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material->Get(AI_MATKEY_SHININESS_STRENGTH, roughness);

    std :: cout << "------*------*------*\n";
    std :: cout << "Material name : " << mat_name.C_Str() << std::endl;
    std :: cout << "Color : " << color.r << " " << color.g << " " << color.b << "\n";
    std :: cout << "Specular : " << specular.r << " " << specular.g << " " << specular.b << "\n";
    std :: cout << "Reflective : " << reflective.r << " " << reflective.g << " " << reflective.b << "\n";
    std :: cout << "Roughness : " << roughness << std :: endl;

    aiTextureType texType = aiTextureType_DIFFUSE;
    for (unsigned i = 0; i < material->GetTextureCount(texType); i++){
        aiString str;
        material->GetTexture(texType, i, &str);
        std:: cout << "tex string : " <<  str.C_Str() << "\n";
        // const aiTexture* mTexture = scene->GetEmbeddedTexture(str.C_Str());
    }

    // std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // // 2. specular maps
    // std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // // 3. normal maps
    // std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // // 4. height maps
    // std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    std::cout << "Mesh size " << vertices.size() << ", " << indices.size() << ", " << textures.size() << "\n";

    // return Mesh(vertices, indices, textures);
}
#endif

#ifndef TEST_MAIN
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
    // glfwSwapInterval(0); // Disable v-sync

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* ImGui Init stuff */
    const char *glsl_version = "#version 130";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    Shader mShader("../shaders/vertex.vert", "../shaders/fragment.frag");

    Model scene("../resources/models/backpack/backpack.obj");
    // Model scene("../resources/models/basic_scene.dae");


    glEnable(GL_DEPTH_TEST);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1.0));

    glm::mat3x3 upper_model(model);
    glm::mat3x3 normal_mat = glm::transpose(glm::inverse(upper_model));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos(0.0f, 1.0f, 2.0f);

    mShader.use();
    mShader.setMat4("model", model);
    mShader.setMat3("normalMat", normal_mat);
    mShader.setMat4("projection", projection);

    mShader.setVec3("lightColor", lightColor);
    mShader.setVec3("lightPos", lightPos);

    const float radius = 10.0f;
    float lastFrame = 0.0f;
    float deltaTime;

    float delay = 0.25f; // 0.25 seconds
    float delayPassed = 0.0f;
    float FPS = 0.0;
    float deltaInMS;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        delayPassed += deltaTime;

        processInput(window);

        cam.UpdateCamera(window, deltaTime);
        glm::mat4 view(1.0f);
        view = cam.getCameraViewMatrix();
        glm::vec3 camPos = cam.getPosition();

        mShader.use();
        mShader.setMat4("view", view);
        mShader.setVec3("viewPos", camPos);
        mShader.setVec3("lightColor", lightColor);
        mShader.setVec3("lightPos", lightPos);

        scene.Draw(mShader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow();

        ImGui::Begin("Debug Info");
        if (delayPassed > delay)
        {
            FPS = (1.0 / deltaTime);
            deltaInMS = deltaTime * 1000;
        }
        ImGui::Text("delta Time: %.2f", deltaInMS);
        ImGui::Text("FPS: %.2f", FPS);

        ImGui::SeparatorText("Camera");
        ImGui::Text("Cam Pos: %.2f %.2f %.2f", camPos.x, camPos.y, camPos.z);
        ImGui::SeparatorText("Lighting");
        ImGui::ColorEdit3("Light Color", &lightColor.x);
        ImGui::DragFloat3("Light Position", &lightPos.x, 0.5f);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (delayPassed > delay)
            delayPassed = 0.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseEnable = false;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseEnable = true;
    }
    // glfwSetWindowShouldClose(window, true);
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

    if (mouseEnable)
        cam.ProcessMouseMovement(xoffset, yoffset, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#endif