#!/usr/bin/env bash
set -e

# This builds the project in Release mode.
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# This runs the executable from the most common CMake output locations.
if [ -f ./build/VectorParticleSystem ]; then
    ./build/VectorParticleSystem
elif [ -f ./build/Release/VectorParticleSystem ]; then
    ./build/Release/VectorParticleSystem
else
    echo "Built successfully, but I could not find the executable automatically."
fi
