#include "BlackHole.h"
#include <GLFW/glfw3.h>  // FIX: Wajib untuk glfwGetTime()
#include <cmath>

BlackHole::BlackHole() {
    generateSphere();
    generateAccretionDisk();
}

BlackHole::~BlackHole() {
    cleanup();
    delete sphereShader;
    delete diskShader;
}

void BlackHole::generateSphere() {
    int sec = 32, stck = 32;
    for (int i = 0; i <= stck; ++i) {
        float phi = i * 3.14159f / stck;
        for (int j = 0; j <= sec; ++j) {
            float theta = j * 2 * 3.14159f / sec;
            glm::vec3 p(
                eventHorizonRadius * cos(theta) * sin(phi),
                eventHorizonRadius * sin(theta) * sin(phi),
                eventHorizonRadius * cos(phi)
            );
            sphereVertices.push_back({p, glm::normalize(p)});
        }
    }
    for (int i = 0; i < stck; ++i) {
        for (int j = 0; j < sec; ++j) {
            unsigned int f = i * (sec + 1) + j;
            unsigned int s = f + sec + 1;
            sphereIndices.insert(sphereIndices.end(), {f, s, f + 1, s, s + 1, f + 1});
        }
    }
}

void BlackHole::generateAccretionDisk() {
    int rings = 20;
    int segments = 64;
    float innerRadius = eventHorizonRadius * 1.5f;
    float outerRadius = accretionDiskRadius;
    
    for (int i = 0; i <= rings; ++i) {
        float r = innerRadius + (outerRadius - innerRadius) * (i / (float)rings);
        for (int j = 0; j <= segments; ++j) {
            float theta = j * 2 * 3.14159f / segments;
            float x = r * cos(theta);
            float z = r * sin(theta);
            float y = sin(theta * 3 + i * 0.5f) * 0.1f;
            diskVertices.push_back({glm::vec3(x, y, z), glm::vec3(0, 1, 0)});
        }
    }
    
    for (int i = 0; i < rings; ++i) {
        for (int j = 0; j < segments; ++j) {
            unsigned int tl = i * (segments + 1) + j;
            unsigned int tr = tl + 1;
            unsigned int bl = (i + 1) * (segments + 1) + j;
            unsigned int br = bl + 1;
            diskIndices.insert(diskIndices.end(), {tl, bl, tr, tr, bl, br});
        }
    }
}

void BlackHole::setupOpenGL() {
    sphereShader = new Shader("shaders/blackhole.vert", "shaders/blackhole.frag");
    
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(Vertex), sphereVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    diskShader = new Shader("shaders/accretion.vert", "shaders/accretion.frag");
    
    glGenVertexArrays(1, &diskVAO);
    glGenBuffers(1, &diskVBO);
    glGenBuffers(1, &diskEBO);
    glBindVertexArray(diskVAO);
    glBindBuffer(GL_ARRAY_BUFFER, diskVBO);
    glBufferData(GL_ARRAY_BUFFER, diskVertices.size() * sizeof(Vertex), diskVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, diskEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, diskIndices.size() * sizeof(unsigned int), diskIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
}

void BlackHole::update(float dt) {
    if (rotate) rotation += dt * 0.3f;
}

void BlackHole::render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos) {
    // Render accretion disk
    diskShader->use();
    glm::mat4 diskModel = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0));
    diskShader->setMat4("model", diskModel);
    diskShader->setMat4("view", view);
    diskShader->setMat4("projection", proj);
    diskShader->setVec3("viewPos", viewPos);
    diskShader->setFloat("time", (float)glfwGetTime()); // Sekarang akan dikenali
    glBindVertexArray(diskVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(diskIndices.size()), GL_UNSIGNED_INT, 0);
    
    // Render event horizon (sphere hitam)
    sphereShader->use();
    glm::mat4 sphereModel = glm::mat4(1.0f);
    sphereShader->setMat4("model", sphereModel);
    sphereShader->setMat4("view", view);
    sphereShader->setMat4("projection", proj);
    sphereShader->setVec3("viewPos", viewPos);
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, 0);
}

void BlackHole::cleanup() {
    if(sphereVAO) glDeleteVertexArrays(1, &sphereVAO);
    if(sphereVBO) glDeleteBuffers(1, &sphereVBO);
    if(sphereEBO) glDeleteBuffers(1, &sphereEBO);
    if(diskVAO) glDeleteVertexArrays(1, &diskVAO);
    if(diskVBO) glDeleteBuffers(1, &diskVBO);
    if(diskEBO) glDeleteBuffers(1, &diskEBO);
    sphereVAO = sphereVBO = sphereEBO = 0;
    diskVAO = diskVBO = diskEBO = 0;
}