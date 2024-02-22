#include <camera.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 &position, float Yaw = -90.0f, float Pitch = 0.0f) : Position(position)
{
    // view = glm::lookAt(Position, Position + Front, worldUp);
    yaw = Yaw;
    pitch = Pitch;
    updateCameraVectors();
}

void Camera::UpdateCamera(GLFWwindow *window, float deltaTime)
{
    // Process keyboard input
    float velocity = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        velocity *= 2.5f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        velocity *= 0.2f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += velocity * Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= velocity * Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= glm::normalize(glm::cross(Front, worldUp)) *
                    velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += glm::normalize(glm::cross(Front, worldUp)) *
                    velocity;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        Position -= worldUp * velocity;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Position += worldUp * velocity;

    // ProcessMouseMovement();
}

glm::mat4 Camera::getCameraViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, worldUp);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, worldUp));
    cameraUp = glm::normalize(glm::cross(Right, Front));
}
