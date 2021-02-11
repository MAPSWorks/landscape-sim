//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan command pool object.
#ifndef LSIM_RENDERER_VLK_COMMAND_POOL_H_
#define LSIM_RENDERER_VLK_COMMAND_POOL_H_
#include <type_traits>

#include <vulkan/vulkan.h>

#include "device_queue.h"

namespace lsim::renderer::vlk {
class CommandPool {
public:
  // Flags of a command buffer
  enum class Flags { kNone = 0, kResetable = 1 << 0, kTransient = 1 << 1 };

  // is_transient - true for short-lived buffers for optimization
  CommandPool(const VkDevice &device, QueueFamilyIndex family_index,
              Flags flags = Flags::kNone);
  ~CommandPool();
  CommandPool(CommandPool const &) = delete;
  CommandPool operator=(CommandPool const &) = delete;
  const VkCommandPool &Get() const;
  // VkCommandBuffer AllocateCommandPrimaryBuffer() const;

private:
  VkCommandPool Create(QueueFamilyIndex family_index, Flags flags) const;
  // Reference to resource this object is created with
  const VkDevice &device_;
  const VkCommandPool command_pool_ = VK_NULL_HANDLE;
};

// Comparison operators for flag
inline CommandPool::Flags operator|(CommandPool::Flags lhs,
                                    CommandPool::Flags rhs) {
  return static_cast<CommandPool::Flags>(
      static_cast<std::underlying_type<CommandPool::Flags>::type>(lhs) |
      static_cast<std::underlying_type<CommandPool::Flags>::type>(rhs));
}
inline CommandPool::Flags operator&(CommandPool::Flags lhs,
                                    CommandPool::Flags rhs) {
  return static_cast<CommandPool::Flags>(
      static_cast<std::underlying_type<CommandPool::Flags>::type>(lhs) &
      static_cast<std::underlying_type<CommandPool::Flags>::type>(rhs));
}
} // namespace lsim::renderer::vlk

#endif
