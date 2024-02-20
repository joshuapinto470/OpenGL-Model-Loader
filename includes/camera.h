#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(glm::vec3 &, float, float);
    void UpdateCamera(GLFWwindow *, float);
    void ProcessMouseMovement(float, float, GLboolean);
    glm::mat4 getCameraViewMatrix() const;

private:
    void updateCameraVectors();
    // glm::mat4 view;
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right;
    glm::vec3 cameraUp;

    float cameraSpeed = 5.0f;
    float MouseSensitivity = 0.1f;

    float yaw;
    float pitch;
    float lastX = 1200.0f / 2.0;
    float lastY = 800.0 / 2.0;
};