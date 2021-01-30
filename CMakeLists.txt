cmake_minimum_required(VERSION 3.16)

project(landscape-sim
        VERSION 0.1
        DESCRIPTION "Landscape simulation engine with applications"
        LANGUAGES CXX)

# Libraries should be provided by the system
find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)
# If glm_DIR is not set it will be taken from the system
set(glm_DIR extern/glm/cmake/glm)
find_package(glm REQUIRED)

add_subdirectory(src/renderer)
add_subdirectory(src/platform)
add_subdirectory(apps)
