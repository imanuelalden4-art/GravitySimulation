#include "NeutronStar.h"
#include <glm/gtc/matrix_transform.hpp> // FIX: Wajib untuk glm::rotate

NeutronStar::NeutronStar() : radius(1.5f), rotation(0.0f), rotate(true), shader(nullptr) {
    generateSphere();
}

NeutronStar::~NeutronStar() {
    cleanup();
    delete shader;
}

void NeutronStar::generateSphere() {
    int sec = 32, stck = 32;
    for (int i = 0; i <= stck; ++i) {
        float phi = i * 3.14159f / stck;
        for (int j = 0; j <= sec; ++j) {
            float theta = j * 2 * 3.14159f / sec;
            glm::vec3 p(radius * cos(theta) * sin(phi), radius * sin(theta) * sin(phi), radius * cos(phi));
            vertices.push_back({p, glm::normalize(p)});
        }
    }
    for (int i = 0; i < stck; ++i) {
        for (int j = 0; j < sec; ++j) {
            // FIX: Langsung deklarasikan sebagai unsigned int
            unsigned int f = i * (sec + 1) + j;
            unsigned int s = f + sec + 1;
            indices.insert(indices.end(), {f, s, f + 1, s, s + 1, f + 1});
        }
    }
}

void NeutronStar::setupOpenGL() {
    shader = new Shader("shaders/star.vert", "shaders/star.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
}

void NeutronStar::update(float dt) {
    if (rotate) rotation += dt * 0.5f;
}

void NeutronStar::render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos) {
    shader->use();
    // FIX: glm::rotate sekarang akan dikenali
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0));
    
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", proj);
    shader->setVec3("viewPos", viewPos);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void NeutronStar::cleanup() {
    if(VAO) glDeleteVertexArrays(1, &VAO);
    if(VBO) glDeleteBuffers(1, &VBO);
    if(EBO) glDeleteBuffers(1, &EBO);
    VAO = VBO = EBO = 0;
}