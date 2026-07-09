cmake_minimum_required(VERSION 3.18)

# This is the project name that shows up in the build folder.
project(VectorParticleSystem LANGUAGES CXX)

# I use C++17 because it is modern but still easy to run on most computers.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# This lets CMake download raylib the first time the project is built.
include(FetchContent)

# These options keep raylib from building extra sample projects.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

# raylib handles the window, drawing, keyboard, and mouse input.
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)

# This adds raylib to my project so I can link against it.
FetchContent_MakeAvailable(raylib)

# This is my actual program file.
add_executable(VectorParticleSystem src/main.cpp)

# This connects the program with raylib.
target_link_libraries(VectorParticleSystem PRIVATE raylib)

# This helps Visual Studio open the right target first.
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT VectorParticleSystem)
