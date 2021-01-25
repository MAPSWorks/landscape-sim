
add_executable(terrain main.cc)

target_include_directories(terrain PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(terrain PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(terrain PUBLIC vulkan)
