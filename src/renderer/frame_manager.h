#pragma once
#include <deque>
#include <base/types.h>
#include "vlk/command_pool.h"
#include "frame_resource.h"

// Stores and manages frame resources
namespace renderer {
class FrameManager {
public:
    using FrameId = t::U32;
    FrameManager(const VkDevice& device, t::U32 family_index, t::U32 frames_in_flight);
    // Sets the frame resources in their default state 
    // as if rendering for the first time
    // TODO: not fully implemented, check comments inside a function
    void Reset();
    // Updates resource index
    void Update();
    // Return currently processed frame resource
    FrameResource& GetCurrentFrameResource();
    // Index of the rame-in-flight currently processed
    FrameId GetCurrentFrameIndex() const;
private:
    std::deque<FrameResource> GetFrameResources(const VkDevice& device) const;
    // Number of total frames that are to be processed in paralel
    const t::U32 kFramesInFlight;
    // Command buffers are allocated from this pool
    vlk::CommandPool command_pool_;
    // Deque is used because objects have no copy or move constructors
    // Alternatives - store unique_ptr in vector or define move constructor
    std::deque<FrameResource> frame_resources_;
    // Index of currently processed frame resource.
    // It goes from 0 to number of frame resources
    FrameId frame_index_ = 0;
};
}; // renderer