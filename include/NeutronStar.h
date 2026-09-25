#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

struct Vertex {
    glm::vec3 pos, normal;
};

class NeutronStar {
public:
    float radius;
    float rotation;
    bool rotate;
    unsigned int VAO, VBO, EBO;
    Shader* shader;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    NeutronStar();
    ~NeutronStar();
    void generateSphere();
    void setupOpenGL();
    void update(float dt);
    void render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos);
    void cleanup();
};