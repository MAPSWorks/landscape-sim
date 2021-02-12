//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/command_pool.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/device_queue.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
CommandPool::CommandPool(const VkDevice &device, QueueFamilyIndex family_index,
                         Flags flags)
    : device_(device), command_pool_(Create(family_index, flags)) {
  base::Log::Info("renderer", "command pool", "created");
}

// Command buffers are destroyed together with pool
CommandPool::~CommandPool() {
  base::Log::Info("renderer", "command pool (and implicitly command buffers)",
                  "destroying..");
  vkDestroyCommandPool(device_, command_pool_, nullptr);
}

const VkCommandPool &CommandPool::Handle() const { return command_pool_; }

/*
// Command buffer alloceted will be destroyed implicitly with pool destruction
VkCommandBuffer CommandPool::AllocateCommandPrimaryBuffer() const {
  VkCommandBufferAllocateInfo command_buffer_allocinfo{};
  command_buffer_allocinfo.sType =
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_allocinfo.commandPool = command_pool_;
  command_buffer_allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_allocinfo.commandBufferCount = 1;
  VkCommandBuffer command_buffer;
  ErrorCheck(vkAllocateCommandBuffers(device_, &command_buffer_allocinfo,
                                      &command_buffer));
  return command_buffer;
}
*/

VkCommandPool CommandPool::Create(QueueFamilyIndex family_index,
                                  Flags flags) const {
  VkCommandPoolCreateInfo pool_create_info{};
  pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_create_info.queueFamilyIndex = family_index;
  pool_create_info.flags = 0;
  // Command buffer can be re-recorded
  if ((flags & Flags::kResetable) == Flags::kResetable) {
    pool_create_info.flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  }
  // for short-lived buffers
  if ((flags & Flags::kTransient) == Flags::kTransient) {
    pool_create_info.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
  }
  base::Log::Info("renderer", "command pool", "queue family index set to",
                  family_index);
  VkCommandPool command_pool;
  ErrorCheck(
      vkCreateCommandPool(device_, &pool_create_info, nullptr, &command_pool));
  return command_pool;
}
} // namespace lsim::renderer::vlk