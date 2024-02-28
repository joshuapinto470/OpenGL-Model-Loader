#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
   public:
    /// @brief Camera class
    /// @param position A glm::vec3 position vector (sets the starting positon of the
    /// camera)
    /// @param attitude A glm::vec2 vector which defines the yaw and pitch of the camera
    /// (yaw, pitch)
    /// @param clip A glm::vec2 defining the near and far plane of the camera.
    /// @param aspect The aspect ration of the camear (usually SCREEN_WIDTH /
    /// SCREEN_HEIGHT)
    Camera(glm::vec3, glm::vec2, glm::vec2, float);
    void UpdateCamera(GLFWwindow *, float);
    void ProcessMouseMovement(float, float, GLboolean, bool);
    void UpdateProjection(float, float);
    glm::mat4 getCameraViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;

   private:
    void updateCameraVectors();
    // glm::mat4 view;
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right;
    glm::vec3 cameraUp;

    glm::mat4 m_projection;

    // Projection matrix variables
    float FOV = 45.0f;
    float near_clip, far_clip;
    float aspect;

    // attitude transform control variables
    float cameraSpeed = 5.0f;
    float MouseSensitivity = 0.1f;

    float yaw;
    float pitch;
    float lastX = 1200.0f / 2.0;
    float lastY = 800.0 / 2.0;
    bool firstMouse = true;
};