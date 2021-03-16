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

Fence::~Fence() {
  base::Log::Info("renderer", "fence", "destroying..");
  vkDestroyFence(context_device_, fence_, nullptr);
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
} // namespace lsim::renderer::vlk