#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(glm::vec3 &);
    void UpdateCamera(GLFWwindow *, float);
    glm::mat4 getCameraViewMatrix() const;

private:
    glm::mat4 view;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float cameraSpeed = 0.05f;

    bool firstMouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 1200.0f / 2.0;
    float lastY = 800.0 / 2.0;
};