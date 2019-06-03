#pragma once
#include "vlk/command_pool.h"
#include "vlk/command_buffer.h"
#include "vlk/semaphore.h"
#include "vlk/fence.h"
#include "vlk/frame_buffer.h"

// POD
// Collection of resources directly tied to rendering of a single frame
namespace renderer {
struct FrameResource {
    FrameResource(const VkDevice& device, const vlk::CommandPool& command_pool);
    ~FrameResource();
    const vlk::CommandBuffer command_buffer;
    const vlk::Semaphore image_acquired_semaphore;
    const vlk::Semaphore ready_to_present_semaphore;
    const vlk::Fence drawing_finished_fence;
    // ... Depth attachment
    // In this scenario frame buffer is recreated every frame for a given combination of render pass
    // and image vie and its size. This is not optimal but reduces complexity
    // for storing all kinds of framebuffers and resizing them.
    // Alternatives:
    // * pre-create all buffers and resize them
    // * create framebuffer manager with catche that stores already created framebuffers in catche
    // and adds only new ones
    vlk::FrameBuffer frame_buffer;
    // Swapchain image index that is acquired during renderign current frame
    uint32_t image_index = 0;
};
}; // renderer