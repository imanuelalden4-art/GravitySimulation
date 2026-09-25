#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class SpacetimeGrid {
public:
    int resolution;
    float size;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    SpacetimeGrid(int res, float sz);
    ~SpacetimeGrid();
    void generateMesh();
    void updateDeformation(float mass, float curvature);
    void setupOpenGL();
    void render();
    void cleanup();
};