#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

struct Particle {
    glm::vec3 pos, vel;
    std::vector<glm::vec3> trail;
    bool active = true;
};

class ParticleSystem {
public:
    std::vector<Particle> particles;
    bool enabled = true, showTrails = true;
    unsigned int VAO = 0, VBO = 0, trailVAO = 0, trailVBO = 0;
    Shader *pShader = nullptr, *tShader = nullptr;

    ParticleSystem() {}
    ~ParticleSystem() { cleanup(); delete pShader; delete tShader; }

    void init(int count, float mass) {
        particles.clear();
        for (int i = 0; i < count; ++i) {
            float angle = (i / (float)count) * 2 * 3.14159f;
            float dist = 6.0f + (i % 3) * 3.0f;
            Particle p;
            p.pos = glm::vec3(cos(angle) * dist, 0.0f, sin(angle) * dist);
            float v = sqrt(mass / dist);
            p.vel = glm::vec3(-sin(angle) * v, 0.0f, cos(angle) * v);
            p.active = true;
            particles.push_back(p);
        }
    }

    void update(float dt, float mass, bool isBlackHole) {
        if (!enabled) return;
        
        float eventHorizon = isBlackHole ? 1.0f : 0.5f;
        
        for (auto& p : particles) {
            if (!p.active) continue;
            
            glm::vec3 dir = -p.pos;
            float r2 = glm::dot(dir, dir);
            float r = sqrt(r2);
            
            if (r2 > 0.5f) {
                // Gravitasi lebih kuat untuk Black Hole
                float gravityMultiplier = isBlackHole ? 3.0f : 1.0f;
                p.vel += (mass * gravityMultiplier / r2) * glm::normalize(dir) * dt;
                p.pos += p.vel * dt;
                
                // Black Hole: partikel terhisap kalau terlalu dekat
                if (isBlackHole && r < eventHorizon) {
                    p.active = false;
                    // Respawn particle setelah beberapa saat
                    float angle = (rand() % 360) * 3.14159f / 180.0f;
                    float dist = 8.0f + (rand() % 5);
                    p.pos = glm::vec3(cos(angle) * dist, 0.0f, sin(angle) * dist);
                    float v = sqrt(mass / dist);
                    p.vel = glm::vec3(-sin(angle) * v, 0.0f, cos(angle) * v);
                    p.active = true;
                    p.trail.clear();
                }
                
                if (showTrails) {
                    p.trail.push_back(p.pos);
                    if (p.trail.size() > 300) p.trail.erase(p.trail.begin());
                }
            }
        }
    }

    void setupOpenGL() {
        pShader = new Shader("shaders/particle.vert", "shaders/particle.frag");
        tShader = new Shader("shaders/particle.vert", "shaders/particle.frag");
        
        glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glGenVertexArrays(1, &trailVAO); glGenBuffers(1, &trailVBO);
        glBindVertexArray(trailVAO);
        glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
        glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void render(const glm::mat4& view, const glm::mat4& proj) {
        if (!enabled) return;
        pShader->use(); pShader->setMat4("view", view); pShader->setMat4("projection", proj);
        glBindVertexArray(VAO);
        std::vector<glm::vec3> posData;
        for (auto& p : particles) {
            if (p.active) posData.push_back(p.pos);
        }
        if (posData.size() > 0) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, posData.size() * sizeof(glm::vec3), posData.data());
            glDrawArrays(GL_POINTS, 0, posData.size());
        }

        if (showTrails) {
            tShader->use(); tShader->setMat4("view", view); tShader->setMat4("projection", proj);
            glBindVertexArray(trailVAO);
            for (auto& p : particles) {
                if (p.active && p.trail.size() > 1) {
                    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, p.trail.size() * sizeof(glm::vec3), p.trail.data());
                    glDrawArrays(GL_LINE_STRIP, 0, p.trail.size());
                }
            }
        }
    }

    void cleanup() {
        if(VAO) glDeleteVertexArrays(1, &VAO); if(VBO) glDeleteBuffers(1, &VBO);
        if(trailVAO) glDeleteVertexArrays(1, &trailVAO); if(trailVBO) glDeleteBuffers(1, &trailVBO);
    }
};