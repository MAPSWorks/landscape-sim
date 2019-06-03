#include "frame_resource.h"
#include <base/log.h>

namespace renderer {
FrameResource::FrameResource(const VkDevice& device, const vlk::CommandPool& command_pool) :
    command_buffer(command_pool),
    image_acquired_semaphore(device),
    ready_to_present_semaphore(device),
    drawing_finished_fence(device),
    frame_buffer(device) {
    base::Log::Info("Renderer: frame resource initialized");
}

FrameResource::~FrameResource() {
    base::Log::Info("Renderer: frame resource shutting down...");
}

}; // renderer