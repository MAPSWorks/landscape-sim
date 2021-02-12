//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/semaphore.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
Semaphore::Semaphore(const VkDevice &device)
    : device_(device), semaphore_(Create()) {
  base::Log::Info("renderer", "semaphore", "created");
}

Semaphore::~Semaphore() {
  base::Log::Info("renderer", "semaphore", "destroying..");
  vkDestroySemaphore(device_, semaphore_, nullptr);
}

const VkSemaphore &Semaphore::Handle() const { return semaphore_; }

VkSemaphore Semaphore::Create() const {
  VkSemaphoreCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  VkSemaphore semaphore;
  ErrorCheck(vkCreateSemaphore(device_, &create_info, nullptr, &semaphore));
  return semaphore;
}
} // namespace lsim::renderer::vlk