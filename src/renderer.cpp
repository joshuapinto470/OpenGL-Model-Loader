#include <renderer.h>

#include <iostream>

namespace OGML {
Render::Render(unsigned W, unsigned H) : SCR_WIDTH(W), SCR_HEIGHT(H) {
    pWindow = initRenderer();
    if (!pWindow) {
        std::cout << "ERROR::Failed to init renderer." << std::endl;
    }
    m_model = nullptr;
    m_camera = nullptr;
}

GLFWwindow* Render::initRenderer() {
    if (!glfwInit()) {
        std ::cout << "Failed to init GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Model Loader", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, static_cast<void*>(this));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
    glfwSetCursorPosCallback(window, mouse_cb);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR::Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

void Render::Draw() {
    if (!m_camera) {
        std ::cout << "No camera attached to current renderer." << std::endl;
        return;
    }

    if (!m_model) {
        std ::cout << "No model found to render." << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 model_mat = glm::mat4(1.0f);

    glm::mat3x3 upper_model(model_mat);
    glm::mat3x3 normal_mat = glm::transpose(glm::inverse(upper_model));

    m_shader.use();
    m_shader.setMat4("model", model_mat);
    m_shader.setMat3("normalMat", normal_mat);
    m_shader.setMat4("projection", m_camera->getProjectionMatrix());

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos(0.0f, 1.0f, 2.0f);

    m_shader.setVec3("lightColor", lightColor);
    m_shader.setVec3("lightPos", lightPos);

    float deltaTime;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(pWindow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput();
        m_camera->UpdateCamera(pWindow, deltaTime);

        m_shader.setMat4("view", m_camera->getCameraViewMatrix());
        m_shader.setVec3("viewPos", m_camera->getPosition());
        m_shader.setMat4("projection", m_camera->getProjectionMatrix());

        /* Render here */
        m_model->Draw(m_shader);
        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Render::LoadModel(const char* path) { m_model = new Model(path); }

void Render::AttachCamera(Camera* camera) { m_camera = camera; }
void Render::AttachShader(Shader shader) { m_shader = shader; }

void Render::framebuffer_size_cb(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Render* handler = static_cast<Render*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->SCR_HEIGHT = height;
        handler->SCR_WIDTH = width;
        float aspect = (float)width / height;
        float fov = 45.0f;
        handler->m_camera->UpdateProjection(fov, aspect);
    }
}

void Render::mouse_cb(GLFWwindow* window, double x_pos, double y_pos) {
    float xpos = static_cast<float>(x_pos);
    float ypos = static_cast<float>(y_pos);

    Render* handler = static_cast<Render*>(glfwGetWindowUserPointer(window));
    if (handler) {
        handler->m_camera->ProcessMouseMovement(x_pos, y_pos, GL_TRUE,
                                                handler->mouseEnable);
    }
}

void Render::processInput() {
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseEnable = false;
    }

    if (glfwGetKey(pWindow, GLFW_KEY_C) == GLFW_PRESS) {
        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseEnable = true;
    }
    // glfwSetWindowShouldClose(window, true);
}
}  // namespace OGML