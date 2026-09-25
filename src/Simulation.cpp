#include "Simulation.h"

Simulation::Simulation() 
    : mass(1.4f), starRadius(1.5f), curvature(8.0f), gridResolution(100), 
      paused(false), viewMode(ViewMode::PERSPECTIVE), renderMode(RenderMode::NORMAL), 
      selectedObject(ObjectType::NEUTRON_STAR), lastViewMode(ViewMode::PERSPECTIVE),
      grid(gridResolution, 40.0f) 
{
    grid.setupOpenGL();
    star.setupOpenGL();
    blackHole.setupOpenGL();
    particles.init(8, mass);
    particles.setupOpenGL();
}

void Simulation::update(float dt) {
    // Update grid kalau resolusi berubah
    if (grid.resolution != gridResolution) {
        grid.cleanup();
        grid = SpacetimeGrid(gridResolution, 40.0f);
        grid.setupOpenGL();
    }
    
    star.radius = starRadius;
    star.update(dt);
    blackHole.update(dt);
    
    // FIX: Tambahkan argumen ke-3 (isBlackHole)
    bool isBlackHole = (selectedObject == ObjectType::BLACK_HOLE);
    particles.update(dt, mass, isBlackHole);
    
    grid.updateDeformation(mass, curvature);
    
    // FIX: Hanya reset kamera jika viewMode berubah
    if (viewMode != lastViewMode) {
        camera.setViewMode(viewMode);
        lastViewMode = viewMode;
    }
}

void Simulation::reset() {
    mass = 1.4f;
    starRadius = 1.5f;
    curvature = 8.0f;
    gridResolution = 100;
    paused = false;
    viewMode = ViewMode::PERSPECTIVE;
    lastViewMode = ViewMode::PERSPECTIVE;
    renderMode = RenderMode::NORMAL;
    selectedObject = ObjectType::NEUTRON_STAR;
    camera.reset();
    particles.init(8, mass);
}