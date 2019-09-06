#pragma once
#include "vulkan_shared.h"

namespace renderer::vlk {
class Semaphore {
public:
    Semaphore(const VkDevice& device);
    ~Semaphore();
    Semaphore(Semaphore const&) = delete;
    Semaphore operator=(Semaphore const&) = delete;
    const VkSemaphore& Get() const;
private:
    VkSemaphore Create() const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkSemaphore semaphore_ = VK_NULL_HANDLE;
};
};// renderer vlk