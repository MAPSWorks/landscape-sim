set(LIB_NAME lsim-renderer)

#set(RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src CACHE INTERNAL "Renderer directory")

add_library(${LIB_NAME} vlk/instance.cc
                        vlk/vulkan_shared.cc
                        vlk/validation.cc
                        vlk/debug_messenger.cc
                        renderer.cc)

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g  -D _DEBUG)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${LIB_NAME} PUBLIC ${Vulkan_INCLUDE_DIR})
# TODO possible to remove this if base module is dependant on glm
# or rethink if glm types should be together with common types, otherwise we need to include
# glm everywhere where basic types are used
target_include_directories(${LIB_NAME} PUBLIC ${GLM_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} PUBLIC Vulkan::Vulkan)
                        