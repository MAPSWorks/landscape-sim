set(LIB_NAME lsim-renderer)

#set(RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src CACHE INTERNAL "Renderer directory")

add_library(${LIB_NAME} vlk/instance.cc
                        vlk/vulkan_shared.cc
                        renderer.cc)

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g  -D _DEBUG)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
                        