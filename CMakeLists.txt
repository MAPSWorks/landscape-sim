cmake_minimum_required(VERSION 3.16)

project(landscape-sim
        VERSION 0.1
        DESCRIPTION "Landscape simulation engine with applications"
        LANGUAGES CXX)

# External library directory
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/extern)
# Include directory for all project libraries
set(PROJ_INCLUDE_DIR  ${CMAKE_SOURCE_DIR}/include)
# Source directory for project modules
set(PROJ_SOURCE_DIR  ${CMAKE_SOURCE_DIR}/src)
# Source directory for project application source code
set(PROJ_APPS_DIR  ${CMAKE_SOURCE_DIR}/apps)

# Libraries should be provided by the system
find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)
set(glm_DIR ${EXTERNAL_DIR}/glm/cmake/glm)
find_package(glm REQUIRED)

add_subdirectory(${PROJ_SOURCE_DIR}/renderer)
add_subdirectory(${PROJ_SOURCE_DIR}/platform)
add_subdirectory(${PROJ_APPS_DIR})
