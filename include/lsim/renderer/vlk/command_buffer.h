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
  // Command buffer begin usage flags
  enum class Usage {
    // Specifies that each recording of the command buffer will only be
    // submitted once, and the command buffer will be reset and recorded again
    // between each submission.
    kOneTimeSubmit = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    // Specifies that a command buffer can be resubmitted to a queue while it is
    // in the pending state.
    kSimultaniousUse = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
  };
  CommandBuffer(const CommandPool &command_pool);
  const VkCommandBuffer &Handle() const;
  // Start recording to the command buffer
  void Begin(Usage usage) const;
  // End recording to the command buffer
  void End() const;

private:
  // Reference to resource this object is created with
  const CommandPool &command_pool_;
  VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif