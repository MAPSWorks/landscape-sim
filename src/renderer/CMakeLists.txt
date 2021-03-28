set(LIB_NAME lsim-renderer)

#set(RENDERER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src CACHE INTERNAL "Renderer directory")

# Module specific include directory
set(MODULE_DIR  ${PROJ_INCLUDE_DIR}/lsim/renderer)

# Headers are used in certain IDE's
set(HEADERS vlk/vulkan_shared.cc
            ${MODULE_DIR}/vlk/instance.h
            ${MODULE_DIR}/vlk/validation.h
            ${MODULE_DIR}/vlk/debug_messenger.h
            ${MODULE_DIR}/vlk/device.h
            ${MODULE_DIR}/vlk/queue_families.h
            ${MODULE_DIR}/vlk/physical_device.h
            ${MODULE_DIR}/vlk/queue.h
            ${MODULE_DIR}/vlk/surface.h
            ${MODULE_DIR}/vlk/swapchain.h
            ${MODULE_DIR}/vlk/image_view.h
            ${MODULE_DIR}/vlk/shader_module.h
            ${MODULE_DIR}/vlk/pipeline_graphics.h
            ${MODULE_DIR}/vlk/pipeline_layout.h
            ${MODULE_DIR}/vlk/render_pass.h
            ${MODULE_DIR}/vlk/framebuffer.h
            ${MODULE_DIR}/vlk/command_pool.h
            ${MODULE_DIR}/vlk/command_buffer.h
            ${MODULE_DIR}/vlk/semaphore.h
            ${MODULE_DIR}/vlk/fence.h
            ${MODULE_DIR}/renderer.h
)
set(SOURCES vlk/instance.cc
            vlk/vulkan_shared.cc
            vlk/validation.cc
            vlk/debug_messenger.cc
            vlk/device.cc
            vlk/queue_families.cc 
            vlk/physical_device.cc 
            vlk/queue.cc 
            vlk/surface.cc
            vlk/swapchain.cc
            vlk/image_view.cc
            vlk/shader_module.cc
            vlk/pipeline_graphics.cc
            vlk/pipeline_layout.cc  
            vlk/render_pass.cc    
            vlk/framebuffer.cc    
            vlk/command_pool.cc 
            vlk/command_buffer.cc 
            vlk/semaphore.cc 
            vlk/fence.cc
            renderer.cc)

add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${LIB_NAME} PUBLIC /W4 /WX -Zi -D _DEBUG)
else()
    target_compile_options(${LIB_NAME} PUBLIC -Wall -Wextra -pedantic -Werror -g -D _DEBUG)
endif()

target_include_directories(${LIB_NAME} PUBLIC ${PROJ_INCLUDE_DIR})
target_include_directories(${LIB_NAME} PUBLIC ${Vulkan_INCLUDE_DIR})
target_include_directories(${LIB_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
# TODO possible to remove this if base module is dependant on glm
# or rethink if glm types should be together with common types, otherwise we need to include
# glm everywhere where basic types are used
target_include_directories(${LIB_NAME} PUBLIC ${GLM_INCLUDE_DIRS})

target_link_libraries(${LIB_NAME} PUBLIC Vulkan::Vulkan)
target_link_libraries(${LIB_NAME} PRIVATE ${SDL2_LIBRARIES})        