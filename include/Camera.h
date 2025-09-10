#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position;
    float Yaw;
    float Pitch;
    float Distance;

    Camera(float distance = 5.0f, float yaw = -90.0f, float pitch = 0.0f);

    glm::mat4 GetViewMatrix();
    void ProcessMouseMovement(float xoffset, float yoffset, float sensitivity = 0.25f, bool constrainPitch = true);

private:
    void updateCameraVectors();
};