cmake_minimum_required(VERSION 3.16)

project(landscape-sim
        VERSION 0.1
        DESCRIPTION "Landscape simulation engine with applications"
        LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(apps)
