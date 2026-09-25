#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() 
    : position(0.0f, 15.0f, 30.0f),  
      yaw(-90.0f),                   
      pitch(-30.0f),                 
      moveSpeed(20.0f),
      mouseSensitivity(0.15f) {}

glm::vec3 Camera::getFront() const {
    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    return glm::normalize(glm::vec3(
        cos(radPitch) * cos(radYaw),
        sin(radPitch),
        cos(radPitch) * sin(radYaw)
    ));
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(getFront(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Camera::getUp() const {
    return glm::normalize(glm::cross(getRight(), getFront()));
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void Camera::processMouseScroll(float yoffset) {
    position += getFront() * yoffset * 2.0f;
}

void Camera::processKeyboard(int key, float deltaTime) {
    float velocity = moveSpeed * deltaTime;
    glm::vec3 front = getFront();
    glm::vec3 right = getRight();

    if (key == 87) position += front * velocity;
    if (key == 83) position -= front * velocity;
    if (key == 65) position -= right * velocity;
    if (key == 68) position += right * velocity;
    if (key == 81) position.y -= velocity;
    if (key == 69) position.y += velocity;
}

void Camera::setViewMode(ViewMode mode) {
    if (mode == ViewMode::TOP) {
        position = glm::vec3(0.0f, 50.0f, 0.01f);
        pitch = -89.0f;
        yaw = -90.0f;
    } else if (mode == ViewMode::SIDE) {
        position = glm::vec3(50.0f, 10.0f, 0.0f);
        pitch = 0.0f;
        yaw = 180.0f;
    } else { // PERSPECTIVE
        position = glm::vec3(0.0f, 15.0f, 30.0f);
        pitch = -30.0f;
        yaw = -90.0f;
    }
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + getFront(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 500.0f);
}

void Camera::reset() {
    position = glm::vec3(0.0f, 15.0f, 30.0f);
    yaw = -90.0f;
    pitch = -30.0f;
}