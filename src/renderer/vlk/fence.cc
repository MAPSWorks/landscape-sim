//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/fence.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
Fence::Fence(VkDevice device, bool signaled)
    : context_device_(device), fence_(Create(signaled)) {
  base::Log::Info("renderer", "fence", "created");
}

Fence::~Fence() { Destroy(); }

Fence::Fence(Fence &&other) noexcept : context_device_(other.context_device_) {
  // Call move-asignment operator
  *this = std::move(other);
}

Fence &Fence::operator=(Fence &&other) noexcept {
  // Check self-move
  if (this != &other) {
    // Free this resource if initialized
    Destroy();
    // Change handle ownership
    this->fence_ = other.fence_;
    // Remove handle from about-to-deleted object so it doesnt destroy resource
    other.fence_ = VK_NULL_HANDLE;
  }
  return *this;
}

VkFence Fence::Handle() { return fence_; }

void Fence::WaitFor() {
  ErrorCheck(vkWaitForFences(context_device_, 1, &fence_, VK_TRUE,
                             std::numeric_limits<uint64_t>::max()));
}

void Fence::Reset() { ErrorCheck(vkResetFences(context_device_, 1, &fence_)); }

VkFence Fence::Create(bool signaled) const {
  VkFenceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (signaled) {
    // Fence is already in signaled state
    create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  }
  VkFence fence = VK_NULL_HANDLE;
  ErrorCheck(vkCreateFence(context_device_, &create_info, nullptr, &fence));
  return fence;
}

void Fence::Destroy() {
  // Handle must be in valid state
  if (fence_ != VK_NULL_HANDLE) {
    base::Log::Info("renderer", "fence", "destroying..");
    vkDestroyFence(context_device_, fence_, nullptr);
    // Make sure handle is not valid anymore
    fence_ = VK_NULL_HANDLE;
  }
}
} // namespace lsim::renderer::vlk