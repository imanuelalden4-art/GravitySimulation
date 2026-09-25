#include "Renderer.h"
#include "Simulation.h"
#include "Shader.h"
#include <vector>
#include <cstdlib>

Renderer::Renderer() : sfVAO(0), sfVBO(0), sfShader(nullptr), gridShader(nullptr) {}

Renderer::~Renderer() {
    cleanup();
}

void Renderer::init() {
    sfShader = new Shader("shaders/starfield.vert", "shaders/starfield.frag");
    gridShader = new Shader("shaders/grid.vert", "shaders/grid.frag");
    setupStarfield();
}

void Renderer::setupStarfield() {
    std::vector<glm::vec3> stars(2000);
    for (auto& s : stars) {
        s = glm::vec3(
            (rand() % 2000 - 1000) / 5.0f,
            (rand() % 2000 - 1000) / 5.0f,
            (rand() % 2000 - 1000) / 5.0f
        );
    }
    glGenVertexArrays(1, &sfVAO);
    glGenBuffers(1, &sfVBO);
    glBindVertexArray(sfVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sfVBO);
    glBufferData(GL_ARRAY_BUFFER, stars.size() * sizeof(glm::vec3), stars.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::render(Simulation& sim) {
    glm::vec3 camPos = sim.camera.position;
    glm::mat4 view = sim.camera.getViewMatrix();
    glm::mat4 proj = sim.camera.getProjectionMatrix(16.0f / 9.0f);

    // Render Starfield
    sfShader->use();
    sfShader->setMat4("view", view);
    sfShader->setMat4("projection", proj);
    glBindVertexArray(sfVAO);
    glDrawArrays(GL_POINTS, 0, 2000);

    // Render Grid
    gridShader->use();
    gridShader->setMat4("view", view);
    gridShader->setMat4("projection", proj);
    gridShader->setMat4("model", glm::mat4(1.0f));
    sim.grid.render();

    // Render objek sesuai pilihan
    if (sim.selectedObject == ObjectType::BLACK_HOLE) {
        sim.blackHole.render(view, proj, camPos);
    } else {
        sim.star.render(view, proj, camPos);
    }

    // Render Particles
    sim.particles.render(view, proj);
}

void Renderer::cleanup() {
    if (sfVAO) glDeleteVertexArrays(1, &sfVAO);
    if (sfVBO) glDeleteBuffers(1, &sfVBO);
    sfVAO = sfVBO = 0;
    
    delete sfShader;
    delete gridShader;
    sfShader = nullptr;
    gridShader = nullptr;
}