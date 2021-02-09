set(LIB_NAME lsim-renderer)

#set(RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src CACHE INTERNAL "Renderer directory")

set(SOURCES vlk/instance.cc
            vlk/vulkan_shared.cc
            vlk/validation.cc
            vlk/debug_messenger.cc
            vlk/device.cc
            vlk/device_queue.cc
            vlk/surface.cc
            vlk/swapchain.cc
            vlk/image_view.cc
            vlk/shader_module.cc
            vlk/pipeline_graphics.cc
            vlk/pipeline_layout.cc  
            vlk/render_pass.cc         
            renderer.cc)

add_library(${LIB_NAME} ${SOURCES})

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${LIB_NAME} PUBLIC /W4 /WX -Zi -D _DEBUG)
else()
    target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g -D _DEBUG)
endif()

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${LIB_NAME} PUBLIC ${Vulkan_INCLUDE_DIR})
target_include_directories(${LIB_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
# TODO possible to remove this if base module is dependant on glm
# or rethink if glm types should be together with common types, otherwise we need to include
# glm everywhere where basic types are used
target_include_directories(${LIB_NAME} PUBLIC ${GLM_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} PUBLIC Vulkan::Vulkan)
target_link_libraries(${LIB_NAME} PRIVATE ${SDL2_LIBRARIES})        