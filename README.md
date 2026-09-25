# 🌌 3D Neutron Star & Black Hole Spacetime Simulation

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=for-the-badge&logo=cplusplus)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3%20Core-green.svg?style=for-the-badge&logo=opengl)
![CMake](https://img.shields.io/badge/CMake-3.16%2B-red.svg?style=for-the-badge&logo=cmake)
![License](https://img.shields.io/badge/Status-Active%20Development-orange.svg?style=for-the-badge)

A real-time interactive 3D visualization project built with **C++ and OpenGL** that demonstrates the visual concept of gravitational spacetime curvature around extremely massive objects such as **Neutron Stars** and **Black Holes**.

The project combines mathematical visualization, particle motion, 3D rendering, and an interactive user interface to create an educational simulation of how massive objects affect the geometry of their surrounding space.

> ⚠️ **Important Disclaimer:** This project is an educational visualization and is **not a physically accurate General Relativity simulator**. The curved grid is a visual analogy inspired by the classic "rubber-sheet" representation of gravity. Real spacetime is a four-dimensional manifold and does not physically bend "downward" into an external spatial dimension.

---

## ✨ Features

### 🌌 Spacetime Visualization
* **Interactive 3D Grid:** Dynamic mesh representing surrounding space.
* **Real-time Deformation:** Immediate visual response to parameter tweaks.
* **Customizable Mechanics:** Adjustable mass, curvature strength, and grid resolution.
* **Smooth Rendering:** Optimized mesh vertex calculations for fluid visual deformation.

### ⭐ Neutron Star
* **3D Spherical Model:** Detailed physical surface representation.
* **Adjustable Parameters:** Real-time modification of radius and mass.
* **Dynamic Animations:** Axis rotation and test particles forming stable orbits.
* **Curvature Visualizer:** Highlights strong local gravitational wells.

### 🕳️ Black Hole
* **Event Horizon:** Dark spherical core visually absorbing ambient light.
* **Accretion Disk:** Dynamic, rotating disk of matter surrounding the horizon.
* **Particle Capture:** Particles crossing the capture threshold are absorbed and respawned.
* **Deep Gravity Well:** Pronounced spacetime distortion effect.

### ✨ Particle System & Camera
* **Orbital Dynamics:** Simulates test particles subject to gravitational pull.
* **Motion Trails:** Visualizes particle trajectories over time.
* **Free-Flight FPS Camera:** Full 6-DOF movement (WASD + mouse look + height control).
* **Preset Views:** One-click camera snapping (Perspective, Top, Side views).

### 🎛️ Interactive User Interface (Dear ImGui)
* Full real-time control over object selection, mass, radius, grid density, simulation speed (Pause/Resume), and camera angles.

---

## 🧠 How the Simulation Works

### 1. Spacetime Grid & Deformation Model
The surrounding space is initialized as a flat 2D grid in 3D space with initial coordinates $(x, 0, z)$.

```text
Flat Space
────────────────────────────
────────────────────────────

Massive Object
──────────────────╲    ╱────
                   ╲  ╱
                    ╲╱
                    ●

# 🧠 How the Simulation Works

## 1. Spacetime Grid

The simulation represents the surrounding space using a 2D grid rendered in a 3D environment.

Each grid point has an initial position:

```text
(x, 0, z)
````

The simulation modifies the vertical position of each point according to its distance from the massive object.

Conceptually:

```text
Flat Space

────────────────────────────
────────────────────────────
────────────────────────────


Massive Object

──────────────────╲    ╱────
                   ╲  ╱
                    ╲╱
                    ●
```

The deformation creates a visual representation of a gravitational well.

---

## 2. Gravitational Deformation

The current visualization uses a simplified softened potential rather than solving Einstein's field equations.

A simplified model is:

```text
y = -(Curvature × Mass) / (distance + epsilon)
```

Where:

* `y` = vertical displacement of the grid.
* `Curvature` = adjustable deformation strength.
* `Mass` = simulated mass of the object.
* `distance` = distance from the object to the grid point.
* `epsilon` = small value used to prevent division by zero.

This equation is intentionally simplified for visualization and real-time performance.

It should therefore be interpreted as a **visual deformation model**, not as an exact solution of General Relativity.

---

# ⭐ Neutron Star vs 🕳️ Black Hole

The simulation provides two different object types.

## ⭐ Neutron Star

The neutron star is represented as a massive spherical object.

Characteristics:

* Visible physical surface.
* Strong gravitational deformation.
* Rotatable 3D model.
* Orbital particles.
* Particle trails.

Conceptually:

```text
       Particle
          ●
       ↗     ↘
     ●         ●
          ⭐
     ●         ●
       ↘     ↗
```

---

## 🕳️ Black Hole

The black hole is represented using an event horizon and an accretion disk.

Components:

```text
       Accretion Disk
     ╱───────────────╲
    ╱       ●         ╲
   ╲    Event Horizon ╱
    ╲───────────────╱
```

The simulation also applies stronger deformation and simplified particle capture behavior.

Particles that cross a predefined capture radius are removed from their current orbit and respawned according to the simulation rules.

> This particle behavior is a simplified visualization and should not be interpreted as a complete model of black-hole dynamics.

---

# 🏗️ Software Architecture

The project separates simulation logic, rendering, camera control, and UI into different components.

```text
┌──────────────────────────────┐
│          Main Loop            │
└──────────────┬───────────────┘
               │
       ┌───────┴────────┐
       ↓                ↓
 Simulation          Renderer
       │                │
       ├── Grid         ├── OpenGL
       ├── Objects      ├── Shaders
       └── Particles    └── Buffers
       │
       ├───────────────┐
       ↓               ↓
    Camera             UI
       │               │
       └───────┬───────┘
               ↓
          Final Frame
```

### Simulation

Responsible for:

* Simulation state.
* Object parameters.
* Grid deformation.
* Particle physics.
* Orbital calculations.
* Simulation time.

### Renderer

Responsible for:

* OpenGL rendering.
* VAO/VBO management.
* Shader management.
* Drawing the grid.
* Drawing objects.
* Drawing particles.
* Drawing the background.

### Camera

Responsible for:

* View matrix.
* Projection matrix.
* FPS-style movement.
* Mouse look.
* Camera presets.

### UI

Uses **Dear ImGui** to provide real-time controls for the simulation.

---

# 📁 Project Structure

```text
NeutronStarSimulation/
│
├── CMakeLists.txt
├── README.md
│
├── include/
│   ├── Camera.h
│   ├── BlackHole.h
│   ├── NeutronStar.h
│   ├── ParticleSystem.h
│   ├── Renderer.h
│   ├── Shader.h
│   ├── Simulation.h
│   ├── SpacetimeGrid.h
│   ├── UI.h
│   │
│   ├── glad/
│   │   └── glad.h
│   │
│   └── KHR/
│       └── khrplatform.h
│
├── src/
│   ├── main.cpp
│   ├── Camera.cpp
│   ├── BlackHole.cpp
│   ├── NeutronStar.cpp
│   ├── ParticleSystem.cpp
│   ├── Renderer.cpp
│   ├── Shader.cpp
│   ├── Simulation.cpp
│   ├── SpacetimeGrid.cpp
│   ├── UI.cpp
│   └── glad.c
│
└── shaders/
    ├── grid.vert
    ├── grid.frag
    ├── star.vert
    ├── star.frag
    ├── blackhole.vert
    ├── blackhole.frag
    ├── accretion.vert
    ├── accretion.frag
    ├── particle.vert
    ├── particle.frag
    ├── starfield.vert
    └── starfield.frag
```

---

# 🛠️ Requirements

## Required Software

### C++ Compiler

You need a C++ compiler. You can use either:

* **Visual Studio 2022 / MSVC**
* **MinGW-w64 / GCC**

You do **not** need both.

### CMake

CMake is used as the project's build system.

Download:

[https://cmake.org/download/](https://cmake.org/download/)

Recommended: **CMake 3.16 or newer**

During installation, enable:

```text
Add CMake to the system PATH
```

### Git

Git is recommended for downloading and managing project dependencies.

Download:

[https://git-scm.com/](https://git-scm.com/)

### Visual Studio 2022

If you choose MSVC as your compiler, install the following workload:

```text
Desktop development with C++
```

Make sure the following components are available:

```text
MSVC Build Tools
Windows SDK
CMake tools for Windows
```

### MinGW-w64

If you choose GCC instead of MSVC, install a modern **MinGW-w64** distribution.

MinGW is only required if you choose the MinGW toolchain.

---

# 📦 Dependencies

The project uses several external libraries.

| Library        | Purpose                                    |
| -------------- | ------------------------------------------ |
| **OpenGL**     | 3D graphics rendering                      |
| **GLAD**       | OpenGL function loader                     |
| **GLFW**       | Window creation, input, and OpenGL context |
| **GLM**        | Mathematics and matrix/vector calculations |
| **Dear ImGui** | Interactive graphical interface            |
| **CMake**      | Build system                               |

Some dependencies can be downloaded automatically through the project's CMake configuration.

---

# 🔌 GLAD Setup

GLAD is used to load modern OpenGL functions.

Generate a GLAD package using a GLAD generator compatible with the project configuration.

Recommended configuration:

```text
Language        : C/C++
Specification   : OpenGL
Profile         : Core
OpenGL Version  : 3.3
Generator       : Local
Generate Loader : Enabled
```

After generating and extracting the package, place the files into the project:

```text
glad.h
    ↓
include/glad/glad.h

khrplatform.h
    ↓
include/KHR/khrplatform.h

glad.c
    ↓
src/glad.c
```

The final structure should look like:

```text
NeutronStarSimulation/
│
├── include/
│   ├── glad/
│   │   └── glad.h
│   │
│   └── KHR/
│       └── khrplatform.h
│
└── src/
    └── glad.c
```

---

# 🔨 Build Instructions

## Option A — Visual Studio / MSVC

Open PowerShell in the project root directory.

Create a build directory:

```powershell
mkdir build
cd build
```

Configure the project:

```powershell
cmake ..
```

Build the project:

```powershell
cmake --build . --config Release
```

The executable will be generated inside the build directory according to the CMake configuration.

For a typical Visual Studio generator, run:

```powershell
.\Release\NeutronStarSimulation.exe
```

---

## Option B — MinGW

Open PowerShell in the project root directory.

Create a build directory:

```powershell
mkdir build
cd build
```

Generate MinGW Makefiles:

```powershell
cmake -G "MinGW Makefiles" ..
```

Compile the project:

```powershell
mingw32-make
```

Run the application:

```powershell
.\NeutronStarSimulation.exe
```

---

# 🌐 Dependency Download

Depending on the project's `CMakeLists.txt`, external dependencies such as:

* GLFW
* GLM
* Dear ImGui

may be downloaded or configured automatically during the CMake configuration stage.

Therefore, an active internet connection may be required during the first configuration.

---

# 🎮 Controls

## Camera Controls

The camera uses an FPS-style free-flight system.

| Action           | Control                            |
| ---------------- | ---------------------------------- |
| Look Around      | Right Mouse Button + Move Mouse    |
| Move Forward     | `W`                                |
| Move Backward    | `S`                                |
| Strafe Left      | `A`                                |
| Strafe Right     | `D`                                |
| Move Up          | `E`                                |
| Move Down        | `Q`                                |
| Fast Zoom / Dash | Mouse Wheel                        |
| Release Cursor   | Release Right Mouse Button / `ESC` |

---

# 🎛️ Simulation Controls

The Dear ImGui panel allows parameters to be modified while the simulation is running.

### Object Type

Switch between:

```text
⭐ Neutron Star
🕳️ Black Hole
```

### Mass

Controls the simulated mass parameter.

Increasing the value increases the strength of the grid deformation according to the simplified mathematical model.

### Radius

Controls the visual radius of the simulated object.

### Curvature

Controls the strength of the visual spacetime deformation.

This parameter is intentionally exposed so the effect can be easily observed.

### Grid Resolution

Controls the number of grid divisions.

```text
25  → Lower detail / better performance
100 → Higher detail
200 → Very high detail / higher GPU workload
```

### Rotate Object

Enables or disables object rotation.

### Particles

Enables or disables the orbital particle system.

### Orbit Trails

Displays the previous positions of particles to visualize their trajectories.

### Camera Views

Provides predefined camera positions such as:

* Perspective
* Top
* Side

### Simulation Controls

```text
Pause
Resume
Reset
```

These controls manage simulation time and restore the default state.

---

# 🧮 Mathematical Model

The project intentionally uses simplified mathematics for real-time visualization.

The grid deformation follows the conceptual relationship:

```text
y = -(C × M) / (d + ε)
```

Where:

```text
C = Curvature
M = Mass
d = Distance
ε = Softening value
```

The `epsilon` value prevents numerical instability when the distance approaches zero.

The model is useful for creating an intuitive visual relationship:

```text
Greater Mass
     ↓
Greater Deformation

Greater Curvature
     ↓
Greater Deformation
```

However, this should **not** be interpreted as the Schwarzschild metric, Einstein field equations, or a numerical General Relativity solver.

---

# ⚠️ Scientific Scope & Limitations

This project is primarily a **graphics and physics-visualization experiment**.

It does not currently attempt to accurately simulate:

* Full General Relativity.
* Einstein field equations.
* 4D spacetime geometry.
* Schwarzschild/Kerr spacetime metrics.
* Gravitational lensing.
* Relativistic time dilation.
* Frame dragging.
* Accurate neutron-star equations of state.
* Relativistic accretion-disk physics.
* General-relativistic particle trajectories.

Instead, the project uses simplified mathematical models designed to provide an intuitive visual representation.

This distinction is important because the familiar "rubber sheet" model is an analogy rather than a literal representation of how spacetime exists in physical reality.

---

# 🚀 Future Development

Possible future improvements include:

* [ ] More physically accurate gravitational models.
* [ ] Schwarzschild metric visualization.
* [ ] Kerr black hole rotation.
* [ ] Gravitational lensing.
* [ ] Relativistic particle trajectories.
* [ ] Relativistic time dilation visualization.
* [ ] More realistic accretion disk rendering.
* [ ] GPU-based grid deformation.
* [ ] Compute shader acceleration.
* [ ] Multiple massive objects.
* [ ] Binary neutron star system.
* [ ] Binary black hole system.
* [ ] Collision simulation.
* [ ] Adjustable simulation time scale.
* [ ] Save/load simulation configurations.
* [ ] Improved visual effects.
* [ ] Performance profiling and optimization.

---

# 🎯 Project Goals

The main goals of this project are:

1. Learn modern C++ project organization.
2. Understand the OpenGL rendering pipeline.
3. Learn how CMake manages a C++ project.
4. Practice 3D mathematics using vectors and matrices.
5. Learn the basics of particle simulation.
6. Visualize gravitational concepts interactively.
7. Explore the relationship between mathematics, physics, and computer graphics.

---

# 📚 Technologies

```text
Language
└── C++

Graphics
└── OpenGL 3.3 Core

Window & Input
└── GLFW

OpenGL Loader
└── GLAD

Mathematics
└── GLM

UI
└── Dear ImGui

Build System
└── CMake

Compiler
├── MSVC
└── MinGW-w64 / GCC

IDE
├── Visual Studio 2022
└── Visual Studio Code
```

---

# 📌 Project Status

**Status:** 🚧 Active Development

The project is currently focused on establishing the core 3D rendering pipeline, spacetime-grid deformation, massive-object visualization, particle behavior, and interactive simulation controls.

As development continues, the simulation may evolve from a primarily visual analogy toward more physically motivated mathematical models.

---

# 🌌 Concept

The central idea of the project can be summarized as:

```text
Mass
 ↓
Gravitational Field
 ↓
Spacetime Geometry
 ↓
Particle Motion
 ↓
Visual Simulation
```

The goal is not simply to create a visually impressive scene, but to use programming and computer graphics as a way to explore the relationship between **physics, mathematics, and simulation**.

```
```
