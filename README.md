# Landscape Simulator
A landscape simulation engine with attention to detail.  


For the old version visit *old-worldtogpu* branch in this repo.

## Technology
Vulkan graphics API and C++17

## Cloning
Use *--recursive* flag when cloning the repository. This will fetch required submodules.

## Building
Build:
> cmake -G "Unix Makefiles" -S . -B build  
> cmake --build build --config Debug --target <target_name>  

where <target_name>  is the name of an application.  

## Project structure
The lsim engine consists of multiple sublibriraries.
- apps - applications using lsim engine
- src - lsim engine source files and non-exported header files
- include - exported header files. To be used in applications or other libraries.
- extern - external modules 

## External dependancies
- Vulkan
- SDL 2
- GLM 

## Author
Designed and developed by [Ivars Rusbergs.](https://github.com/ivarsrb)