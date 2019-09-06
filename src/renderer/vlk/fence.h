#pragma once
#include "vulkan_shared.h"
// For host to gpu synchronization
namespace renderer::vlk {
class Fence {
public:
    // Defaults to already signlaed fence on creation
    Fence(const VkDevice& device, bool signaled = true);
    ~Fence();
    Fence(Fence const&) = delete;
    Fence operator=(Fence const&) = delete;
    const VkFence& Get() const;
    void WaitFor() const;
    void Reset() const;
private:
    VkFence Create(bool signaled) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkFence fence_ = VK_NULL_HANDLE;
};
};// renderer vlk