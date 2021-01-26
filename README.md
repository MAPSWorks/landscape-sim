# Landscape Simulation
A landscape simulation engine and applications using Vulkan API and modern C++

## Cloning
Use *--recursive* flag when cloning the repository. This will fetch required submodules.

## Building
Build:
cmake -G "Unix Makefiles" -S . -B build
cmake --build build --config Debug --target <target_name>

## Project structure
- apps - applications using lsim engine
- src - engine source files and non-exported header files
- include - exported header files. To be used in applications
- extern - external modules 

## External modules
- Vulkan
- SDL 2
- GLM 