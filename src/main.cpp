#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"  // FIX: Wajib untuk ImGuiIO dan ImGui::GetIO()
#include "Simulation.h"
#include "Renderer.h"
#include "UI.h"

// ===== Global Variables =====
float lastX = 500.0f, lastY = 350.0f;
bool firstMouse = true;
float deltaTime = 0.0f, lastFrame = 0.0f;
Simulation* currentSim = nullptr;
int windowWidth = 1000, windowHeight = 700;

// ===== Callback: Window resize =====
void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

// ===== Callback: Mouse scroll (untuk zoom) =====
void scroll_callback(GLFWwindow*, double, double yoffset) {
    if (!currentSim) return;
    currentSim->camera.processMouseScroll((float)yoffset);
}

int main() {
    // ===== Inisialisasi GLFW =====
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(1000, 700, "Neutron Star Spacetime Simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // ===== Inisialisasi GLAD =====
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // ===== Setup Callbacks =====
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // ===== Inisialisasi Objek =====
    Simulation sim;
    currentSim = &sim;
    Renderer renderer;
    UI ui;
    
    renderer.init();
    ui.init(window);
    
    // ===== Setup OpenGL =====
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    std::cout << "========================================" << std::endl;
    std::cout << "  NEUTRON STAR SIMULATION - KAMERA FPS  " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "KLIK KANAN di window = Aktifkan kamera" << std::endl;
    std::cout << "Gerakkan mouse       = Lihat sekeliling" << std::endl;
    std::cout << "W/A/S/D              = Bergerak" << std::endl;
    std::cout << "Q/E                  = Naik/Turun" << std::endl;
    std::cout << "Scroll               = Zoom cepat" << std::endl;
    std::cout << "Lepas klik kanan     = Lepas cursor" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // ===== Main Loop =====
    while (!glfwWindowShouldClose(window)) {
        // Hitung delta time
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (deltaTime > 0.1f) deltaTime = 0.1f;
        
        // ===== KAMERA FPS: Polling manual mouse =====
        bool rightPressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
        
        if (rightPressed) {
            // Aktifkan mode FPS: cursor disembunyikan & di-lock
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            
            // Ambil posisi mouse MANUAL
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            
            if (firstMouse) {
                lastX = (float)xpos;
                lastY = (float)ypos;
                firstMouse = false;
            }
            
            float xoffset = (float)xpos - lastX;
            float yoffset = lastY - (float)ypos; // Reversed: y bottom to top
            
            lastX = (float)xpos;
            lastY = (float)ypos;
            
            // Proses gerakan kamera
            sim.camera.processMouseMovement(xoffset, yoffset);
        } else {
            // Kembalikan cursor normal
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstMouse = true;
        }
        
        // ===== ESC untuk lepas cursor =====
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        
        // ===== Keyboard Input (WASDQE) =====
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                sim.camera.processKeyboard(87, deltaTime); // W
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                sim.camera.processKeyboard(83, deltaTime); // S
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                sim.camera.processKeyboard(65, deltaTime); // A
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                sim.camera.processKeyboard(68, deltaTime); // D
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                sim.camera.processKeyboard(81, deltaTime); // Q
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
                sim.camera.processKeyboard(69, deltaTime); // E
        }
        
        // ===== Update Simulasi =====
        if (!sim.paused) {
            sim.update(deltaTime);
        }
        
        // ===== Render =====
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderer.render(sim);
        ui.render(sim, 1.0f / deltaTime, width, height);
        
        // ===== Swap Buffers =====
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // ===== Cleanup =====
    renderer.cleanup();
    ui.cleanup();
    glfwTerminate();
    
    return 0;
}