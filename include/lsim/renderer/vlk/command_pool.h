//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan command pool object.
#ifndef LSIM_RENDERER_VLK_COMMAND_POOL_H_
#define LSIM_RENDERER_VLK_COMMAND_POOL_H_
#include <vulkan/vulkan.h>

#include "device_queue.h"

namespace lsim::renderer::vlk {
class CommandPool {
public:
  // is_transient - true for short-lived buffers for optimization
  CommandPool(const VkDevice &device, QueueFamilyIndex family_index,
              bool is_resetable, bool is_transient = false);
  ~CommandPool();
  CommandPool(CommandPool const &) = delete;
  CommandPool operator=(CommandPool const &) = delete;
  const VkCommandPool &Get() const;
  //VkCommandBuffer AllocateCommandPrimaryBuffer() const;

private:
  VkCommandPool Create(QueueFamilyIndex family_index, bool is_resetable,
                       bool is_transient) const;
  // Reference to resource this object is created with
  const VkDevice &device_;
  const VkCommandPool command_pool_ = VK_NULL_HANDLE;
};

} // namespace lsim::renderer::vlk

#endif
