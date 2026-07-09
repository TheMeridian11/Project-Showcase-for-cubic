# Vector Physics Particle System

This is a C++ visual math project that shows how forces act on particles. It uses raylib for the graphics window and keeps the vector math in the source code so the math is easy to explain.

## What it demonstrates

- Vector addition: `netForce = gravity + wind + friction`
- Scalar multiplication: `velocity += acceleration * dt`
- Motion update: `position += velocity * dt`
- Visual trails that bend when wind and friction change
- Force arrows for gravity, wind, friction, and the resulting net force

## Controls

- `A / D` or `Left / Right`: change wind
- `Q / E`: decrease / increase gravity
- `1 / 2`: decrease / increase friction
- `F`: toggle friction on/off
- `Space`: create a burst of particles
- `C`: clear particles
- `R`: reset the simulation
- Left mouse click: move the emitter

## How to run

### Requirements

- CMake
- A C++17 compiler
- Internet connection the first time you build, because CMake downloads raylib automatically

### macOS / Linux

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/VectorParticleSystem
```

### Windows PowerShell

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\VectorParticleSystem.exe
```

If you use MinGW instead of Visual Studio on Windows, the executable may be here instead:

```powershell
.\build\VectorParticleSystem.exe
```

## Project files

```text
VectorParticleSystem/
├── CMakeLists.txt
├── README.md
├── build_and_run.bat
├── build_and_run.sh
└── src/
    └── main.cpp
```
