#include "fence.h"
#include <base/log.h>

namespace renderer::vlk {
Fence::Fence(const VkDevice& device) :
    device_(device),
    fence_(Create()) {
    base::Log::Info("Renderer: fence created");
}

Fence::~Fence() {
    base::Log::Info("Renderer: fence destroying...");
    vkDestroyFence(device_, fence_, nullptr);
}

const VkFence& Fence::Get() const {
    return fence_;
}

VkFence Fence::Create() const {
    VkFenceCreateInfo  create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    // Fence is already in signaled state
    create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VkFence fence;
    ErrorCheck(vkCreateFence(device_, &create_info, nullptr, &fence));
    return fence;
}

// Wait for the fence to enter signaled state
void Fence::WaitFor() const {
    vkWaitForFences(device_, 1, &fence_, VK_TRUE, std::numeric_limits<uint64_t>::max());
}

//To set the state of fences to unsignaled from the host
void Fence::Reset() const {
    vkResetFences(device_, 1, &fence_);
}

};// renderer vlk