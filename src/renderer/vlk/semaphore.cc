//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/semaphore.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
Semaphore::Semaphore(VkDevice device)
    : context_device_(device), semaphore_(Create()) {
  base::Log::Info("renderer", "semaphore", "created");
}

Semaphore::~Semaphore() { Destroy(); }

Semaphore::Semaphore(Semaphore &&other) noexcept
    : context_device_(other.context_device_) {
  // Call move-asignment operator
  *this = std::move(other);
}

Semaphore &Semaphore::operator=(Semaphore &&other) noexcept {
  // Check self-move
  if (this != &other) {
    // Free this resource if initialized
    Destroy();
    // Change handle ownership
    this->semaphore_ = other.semaphore_;
    // Remove handle from about-to-deleted object so it doesnt destroy resource
    other.semaphore_ = VK_NULL_HANDLE;
  }
  return *this;
}

VkSemaphore Semaphore::Handle() { return semaphore_; }

VkSemaphore Semaphore::Create() const {
  VkSemaphoreCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  VkSemaphore semaphore = VK_NULL_HANDLE;
  ErrorCheck(
      vkCreateSemaphore(context_device_, &create_info, nullptr, &semaphore));
  return semaphore;
}

// Properly destroy resource
void Semaphore::Destroy() {
  // Handle must be in valid state
  if (semaphore_ != VK_NULL_HANDLE) {
    base::Log::Info("renderer", "semaphore", "destroying..");
    vkDestroySemaphore(context_device_, semaphore_, nullptr);
    // Make sure handle is not valid anymore
    semaphore_ = VK_NULL_HANDLE;
  }
}

} // namespace lsim::renderer::vlk