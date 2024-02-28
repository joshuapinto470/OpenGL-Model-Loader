#include <glad/glad.h>

// #define GLFW_INCLUDE_NONE
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <shader.h>
#include <texture.h>
#include <camera.h>
#include <model.h>
#include <renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char const *argv[]) {
    std ::string path = "";
    if (argc > 0) {
        std ::cout << argc << std ::endl;
        for (int i = 1; i < argc; i++) {
            if (std::strcmp(argv[i], "-m") == 0) {
                path = argv[i + 1];
                std ::cout << "model path : " << argv[i + 1] << std::endl;
                i++;
                continue;
            }
            if (std::strcmp(argv[i], "-s") == 0) {
                std ::cout << "shader path : " << argv[i + 1] << std::endl;
                continue;
            }
        }
        std ::cout << "\n";
    }
    if (path.empty()) {
        path = "../resources/models/Wavefront/Textured Multiobject/Textured Scene.obj";
    }

    OGML::Render renderer(1200, 800);

    Camera *camera = new Camera(glm::vec3(0.0, 3.14, 7.5), glm::vec2(-90.0f, 0.0f),
                                glm::vec2(0.1, 100.0), 1200.0 / 800.0);
    Shader mShader("../shaders/vertex.vert", "../shaders/fragment.frag");

    renderer.LoadModel(path.c_str());
    renderer.AttachCamera(camera);
    renderer.AttachShader(mShader);

    renderer.Draw();

    return 0;
}

#ifdef TEST_MAIN
{
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

    const float radius = 10.0f;
    float lastFrame = 0.0f;
    float deltaTime;

    float delay = 0.25f;  // 0.25 seconds
    float delayPassed = 0.0f;
    float FPS = 0.0;
    float deltaInMS;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        delayPassed += deltaTime;

        scene.Draw(mShader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow();

        ImGui::Begin("Debug Info");
        if (delayPassed > delay) {
            FPS = (1.0 / deltaTime);
            deltaInMS = deltaTime * 1000;
        }
        ImGui::Text("delta Time: %.2f", deltaInMS);
        ImGui::Text("FPS: %.2f", FPS);

        ImGui::SeparatorText("Camera");
        ImGui::Text("Cam Pos: %.2f %.2f %.2f", camPos.x, camPos.y, camPos.z);
        ImGui::Checkbox("Mouse Enable", &mouseEnable);
        ImGui::SeparatorText("Lighting");
        ImGui::ColorEdit3("Light Color", &lightColor.x);
        ImGui::DragFloat3("Light Position", &lightPos.x, 0.5f);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (delayPassed > delay) {
            delayPassed = 0.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwTerminate();
    return 0;
}

#endif