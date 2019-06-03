#include "frame_manager.h"
#include <base/log.h>

namespace renderer {
FrameManager::FrameManager(const VkDevice& device, uint32_t family_index, uint32_t frames_in_flight) :
    kFramesInFlight(frames_in_flight),
    command_pool_(device, family_index, true),
    frame_resources_(GetFrameResources(device)) {
    Reset();
    base::Log::Info("Renderer: frame manager initialized");
}

FrameManager::~FrameManager() {
    base::Log::Info("Renderer: frame manager shutting down...");
}

void FrameManager::Reset() {
    // TODO: dont forget device idle call here
    frame_index_ = 0;
    // TODO the idea was to destroy all frame buffers here
    // and reset all fences to 'signaled' state, but it is not possible
    // to set fence after its creation to forcefully signaled state.
    // Maybe in reset() we should recreate whole frame resource array?
}

// Moves frm 0 to kFramesInFlight
void FrameManager::Update() {
    frame_index_ = (frame_index_ + 1) % kFramesInFlight;
}

FrameResource& FrameManager::GetCurrentFrameResource() {
    return frame_resources_.at(frame_index_);
}

std::deque<FrameResource> FrameManager::GetFrameResources(const VkDevice& device) const {
    std::deque<FrameResource> frame_resources;
    for (uint32_t i = 0; i < kFramesInFlight; i++) {
        frame_resources.emplace_back(device, command_pool_);
    }
    return frame_resources;
}
}; // renderer