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

    float delay = 0.5f; // 2 seconds
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