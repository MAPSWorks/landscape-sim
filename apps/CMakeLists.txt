
add_executable(terrain main.cc)

target_link_libraries(terrain PUBLIC SDL2)
target_link_libraries(terrain PUBLIC vulkan)
