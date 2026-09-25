#include "UI.h"
#include "Simulation.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void UI::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
}

void UI::render(Simulation& sim, float fps, int width, int height) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Controls");
    
    // ===== Pilihan Objek =====
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "=== Object Type ===");
    bool isNeutronStar = (sim.selectedObject == ObjectType::NEUTRON_STAR);
    if (ImGui::RadioButton("Neutron Star", isNeutronStar)) {
        sim.selectedObject = ObjectType::NEUTRON_STAR;
        sim.mass = 1.4f;
        sim.starRadius = 1.5f;
        sim.curvature = 8.0f;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Black Hole", !isNeutronStar)) {
        sim.selectedObject = ObjectType::BLACK_HOLE;
        sim.mass = 10.0f;
        sim.starRadius = 0.8f;
        sim.curvature = 30.0f;
    }
    
    ImGui::Separator();
    
    // ===== Parameter Objek =====
    if (sim.selectedObject == ObjectType::NEUTRON_STAR) {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "=== Neutron Star ===");
    } else {
        ImGui::TextColored(ImVec4(0.5f, 0.0f, 0.8f, 1.0f), "=== Black Hole ===");
    }
    ImGui::SliderFloat("Mass", &sim.mass, 0.1f, 20.0f);
    ImGui::SliderFloat("Radius", &sim.starRadius, 0.3f, 5.0f);
    ImGui::SliderFloat("Curvature", &sim.curvature, 1.0f, 50.0f);
    ImGui::SliderInt("Grid Resolution", &sim.gridResolution, 25, 200);
    
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "=== Display ===");
    ImGui::Checkbox("Rotate Object", &sim.star.rotate);
    ImGui::Checkbox("Particles", &sim.particles.enabled);
    ImGui::Checkbox("Orbit Trails", &sim.particles.showTrails);
    
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "=== Camera Views ===");
    if (ImGui::Button("Perspective")) sim.viewMode = ViewMode::PERSPECTIVE;
    ImGui::SameLine();
    if (ImGui::Button("Top")) sim.viewMode = ViewMode::TOP;
    ImGui::SameLine();
    if (ImGui::Button("Side")) sim.viewMode = ViewMode::SIDE;
    
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "=== Simulation ===");
    if (sim.paused) {
        if (ImGui::Button("Resume")) sim.paused = false;
    } else {
        if (ImGui::Button("Pause")) sim.paused = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Simulation")) sim.reset();
    
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "=== Camera Controls ===");
    ImGui::TextWrapped("Klik KANAN + drag mouse = Lihat sekeliling");
    ImGui::TextWrapped("W/A/S/D = Bergerak");
    ImGui::TextWrapped("Q/E = Naik/Turun");
    ImGui::TextWrapped("Scroll = Zoom cepat");
    ImGui::TextWrapped("ESC = Lepas cursor");
    ImGui::End();

    // 3D Label
    float labelY = sim.selectedObject == ObjectType::BLACK_HOLE ? 2.0f : sim.starRadius + 1.5f;
    const char* labelText = sim.selectedObject == ObjectType::BLACK_HOLE ? "BLACK HOLE" : "NEUTRON STAR";
    
    glm::vec4 worldPos(0.0f, labelY, 0.0f, 1.0f);
    glm::mat4 view = sim.camera.getViewMatrix();
    glm::mat4 proj = sim.camera.getProjectionMatrix((float)width / (float)height);
    glm::vec4 clipPos = proj * view * worldPos;
    
    if (clipPos.w > 0.1f) {
        glm::vec3 ndc = glm::vec3(clipPos) / clipPos.w;
        float sx = (ndc.x * 0.5f + 0.5f) * width;
        float sy = (1.0f - (ndc.y * 0.5f + 0.5f)) * height;
        
        if (sx > 0 && sx < width && sy > 0 && sy < height) {
            ImU32 labelColor = sim.selectedObject == ObjectType::BLACK_HOLE 
                ? IM_COL32(150, 100, 255, 255) 
                : IM_COL32(255, 200, 100, 255);
            ImGui::GetForegroundDrawList()->AddText(
                ImVec2(sx - 50, sy), 
                labelColor, 
                labelText
            );
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}