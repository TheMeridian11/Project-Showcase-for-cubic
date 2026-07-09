@echo off
REM This builds the project in Release mode.
cmake -S . -B build
cmake --build build --config Release

REM This runs the executable from common Windows CMake output locations.
if exist build\Release\VectorParticleSystem.exe (
    build\Release\VectorParticleSystem.exe
) else if exist build\VectorParticleSystem.exe (
    build\VectorParticleSystem.exe
) else (
    echo Built successfully, but I could not find the executable automatically.
)
