#include "frame_resource.h"
#include <base/log.h>

namespace renderer {
FrameResource::FrameResource(const VkDevice& device, const vlk::CommandPool& command_pool, 
    const vlk::MemoryAllocator& allocator, vlk::BufferSize uniform_buffer_size) :
    command_buffer(command_pool),
    uniform_buffer("uniform buffer", allocator, uniform_buffer_size),
    image_acquired_semaphore(device),
    ready_to_present_semaphore(device),
    drawing_finished_fence(device),
    frame_buffer(device) {
    base::Log::Info("Renderer: frame resource initialized");
}
}; // renderer