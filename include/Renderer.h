#pragma once
#include "Simulation.h"
#include "Shader.h"
#include <vector>

class Renderer {
public:
    unsigned int sfVAO, sfVBO;
    Shader* sfShader;
    Shader* gridShader;

    Renderer();
    ~Renderer();
    
    void init();
    void setupStarfield();
    void render(Simulation& sim);
    void cleanup();
};