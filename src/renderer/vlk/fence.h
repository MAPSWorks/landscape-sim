#pragma once
#include "vulkan_shared.h"
// For host to gpu synchronization
namespace renderer::vlk {
class Fence {
public:
    Fence(const VkDevice& device);
    ~Fence();
    Fence(Fence const&) = delete;
    Fence operator=(Fence const&) = delete;
    const VkFence& Get() const;
    void WaitFor() const;
    void Reset() const;
private:
    VkFence Create() const;
    // Reference to resource this object is created with
    const VkDevice& device_;
    const VkFence fence_ = VK_NULL_HANDLE;
};
};// renderer vlk