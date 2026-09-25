#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class ViewMode { PERSPECTIVE, TOP, SIDE };

class Camera {
public:
    glm::vec3 position;
    float yaw;
    float pitch;
    float moveSpeed;
    float mouseSensitivity;

    Camera();

    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    void processKeyboard(int key, float deltaTime);
    void setViewMode(ViewMode mode);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;
    void reset();

    glm::vec3 getFront() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;
};