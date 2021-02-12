//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan command buffer object.
// Command buffer is not created explicitly, but allocated from a command pool
// It is also implicitly destroyed when command pool that created it is
// destroyed.
#ifndef LSIM_RENDERER_VLK_COMMAND_BUFFER_H_
#define LSIM_RENDERER_VLK_COMMAND_BUFFER_H_
#include <vulkan/vulkan.h>

#include "command_pool.h"

namespace lsim::renderer::vlk {
class CommandBuffer {
public:
  CommandBuffer(const CommandPool &command_pool);
  const VkCommandBuffer& Handle() const;
private:
  // Reference to resource this object is created with
  const CommandPool &command_pool_;
  VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif