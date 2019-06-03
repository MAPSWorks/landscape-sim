#pragma once
#include <deque>
#include "vlk/command_pool.h"
#include "frame_resource.h"

// Stores and manages frame resources
namespace renderer {
class FrameManager {
public:
    FrameManager(const VkDevice& device, uint32_t family_index, uint32_t frames_in_flight);
    ~FrameManager();
    // Sets the frame resources in their default state 
    // as if rendering for the first time
    // TODO: not fully implemented, check komments inside a function
    void Reset();
    // Updates resource index
    void Update();
    // Return currently processed frame resource
    FrameResource& GetCurrentFrameResource();
private:
    std::deque<FrameResource> GetFrameResources(const VkDevice& device) const;
    // Number of total frames that are to be processed in paralel
    const uint32_t kFramesInFlight;
    // Command buffers are allocated from this pool
    vlk::CommandPool command_pool_;
    // Deque is used because objects have no copy or move constructors
    // Alternatives - store unique_ptr in vector or define move constructor
    std::deque<FrameResource> frame_resources_;
    // Index of currently processed frame resource.
    // It goes from 0 to number of frame resources
    uint32_t frame_index_ = 0;
};
}; // renderer