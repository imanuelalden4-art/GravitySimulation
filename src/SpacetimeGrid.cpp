#include "SpacetimeGrid.h"
#include <glm/gtc/matrix_transform.hpp>

SpacetimeGrid::SpacetimeGrid(int res, float sz) : resolution(res), size(sz) {
    generateMesh();
}

SpacetimeGrid::~SpacetimeGrid() {
    cleanup();
}

void SpacetimeGrid::generateMesh() {
    vertices.clear();
    indices.clear();
    float step = size / resolution;
    
    // Grid dibuat pada XZ plane (horizontal)
    for (int i = 0; i <= resolution; ++i) {
        for (int j = 0; j <= resolution; ++j) {
            // x dan z untuk grid horizontal, y = 0 (datar)
            float x = -size/2 + j*step;
            float z = -size/2 + i*step;
            vertices.push_back(glm::vec3(x, 0.0f, z));
        }
    }
    
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            unsigned int tl = i * (resolution + 1) + j;
            unsigned int tr = tl + 1;
            unsigned int bl = (i + 1) * (resolution + 1) + j;
            unsigned int br = bl + 1;
            
            indices.insert(indices.end(), {tl, bl, tr, br});
        }
    }
}

void SpacetimeGrid::updateDeformation(float mass, float curvature) {
    float A = curvature * mass;
    float sigma = 2.0f;
    for (auto& v : vertices) {
        // Hitung jarak dari pusat di XZ plane
        float r = glm::length(glm::vec2(v.x, v.z));
        // Deformasi ke bawah (Y negatif)
        v.y = -A / (r + sigma);
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
}

void SpacetimeGrid::setupOpenGL() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void SpacetimeGrid::render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void SpacetimeGrid::cleanup() {
    if(VAO) glDeleteVertexArrays(1, &VAO);
    if(VBO) glDeleteBuffers(1, &VBO);
    if(EBO) glDeleteBuffers(1, &EBO);
    VAO = VBO = EBO = 0;
}