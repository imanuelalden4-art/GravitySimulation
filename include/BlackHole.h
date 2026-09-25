#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"
#include "NeutronStar.h"  // FIX: Include untuk dapat struct Vertex

class BlackHole {
public:
    float eventHorizonRadius = 0.8f;
    float accretionDiskRadius = 4.0f;
    float rotation = 0.0f;
    bool rotate = true;
    
    unsigned int sphereVAO = 0, sphereVBO = 0, sphereEBO = 0;
    unsigned int diskVAO = 0, diskVBO = 0, diskEBO = 0;
    Shader* sphereShader = nullptr;
    Shader* diskShader = nullptr;
    
    std::vector<Vertex> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    std::vector<Vertex> diskVertices;
    std::vector<unsigned int> diskIndices;

    BlackHole();
    ~BlackHole();
    
    void generateSphere();
    void generateAccretionDisk();
    void setupOpenGL();
    void update(float dt);
    void render(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos);
    void cleanup();
};