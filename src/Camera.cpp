#include "Camera.h"

Camera::Camera(float distance, float yaw, float pitch) : Distance(distance), Yaw(yaw), Pitch(pitch)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float sensitivity, bool constrainPitch)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    Position.x = Distance * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Position.y = Distance * sin(glm::radians(Pitch));
    Position.z = Distance * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
}
