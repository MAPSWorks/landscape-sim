# Landscape Simulation
A landscape simulation engine and applications using Vulkan API and modern C++

## Building and running
Build:
cmake -G "Unix Makefiles" -S . -B build
cmake --build build --config Debug --target terrain

Run:
build/apps/terrain

Clean:
rm -rf build

## Project structure
- apps - applications using lsim engine
- src - engine source files and non-exported header files
- include - exported header files. To be used in applications
- extern - external modules (git submodules)