#pragma once
#include "Simulation.h"
#include <GLFW/glfw3.h>

class UI {
public:
    void init(GLFWwindow* window);
    void render(Simulation& sim, float fps, int width, int height);
    void cleanup();
};