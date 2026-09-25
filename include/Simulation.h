#pragma once
#include "SpacetimeGrid.h"
#include "NeutronStar.h"
#include "BlackHole.h"
#include "ParticleSystem.h"
#include "Camera.h"

enum class RenderMode { NORMAL, WIREFRAME, HEIGHT };
enum class ObjectType { NEUTRON_STAR, BLACK_HOLE };

class Simulation {
public:
    float mass;
    float starRadius;
    float curvature;
    int gridResolution;
    bool paused;
    ViewMode viewMode;
    RenderMode renderMode;
    ObjectType selectedObject;
    
    ViewMode lastViewMode;

    SpacetimeGrid grid;
    NeutronStar star;
    BlackHole blackHole;
    ParticleSystem particles;
    Camera camera;

    Simulation();
    void update(float dt);
    void reset();
};