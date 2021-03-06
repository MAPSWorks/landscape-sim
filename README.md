# Landscape Simulator
A landscape simulation engine with attention to detail.  


For the old version visit *old-worldtogpu* branch in this repo.

## Technology
Vulkan graphics API and C++17

## External dependancies
- Vulkan 1.1
- SDL 2
- GLM 

## Supported platforms
Tested on:
- Windows 10
- Ubuntu Linux

## Set up
Install Vulkan SDk 1.1+  
Install SDL2 library as a package on Linux and manually on Windows.  
Make sure Vulkan and SDL are findable by CMake with *find_package()* command.  

Making SDL2 library findable on Windows might take extra steps:  
- create *sdl2-config.cmake* file containing definitions of SDL2_INCLUDE_DIRS and SDL2_LIBRARIES. Both of those variables should point to SDL include and library directories respectively. Make sure SDL include files are searchable at "SDL2/SDL.h" path. 
- at CMake configuration stage add *-DSDL2_DIR="dir_to_sdl2-config.cmake"* definition
- copy SDL2.dll next to application executable file.  

## Cloning and Building
To retrieve the project from github and compile:  
> git clone https://github.com/ivarsrb/landscape-sim.git --recursive  
> cmake -S . -B build  
> cmake --build build --config Debug --target <target_name>   

to compile shaders for the given app target:
> cmake --build build --target <target_name>-shaders   

, where <target_name>  is the name of an application.  

## Project structure
The lsim engine consists of multiple sublibriraries.
- apps - applications using lsim engine
- src - lsim engine source files and non-exported header files
- include - exported header files. To be used in applications or other libraries.
- extern - external modules 

## Coding convention
The project uses Google style guide and C++ Core Guidelines as a coding reference.  
In case Clangd language server is used, *.clang-tidy* file describes the style the code is checked against.  

## Author
Designed and developed by [Ivars Rusbergs.](https://github.com/ivarsrb)