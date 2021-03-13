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
  explicit CommandBuffer(const CommandPool &command_pool);
  [[nodiscard]] VkCommandBuffer Handle();
  // Start recording to the command buffer
  void Begin(Usage usage) const;
  // End recording to the command buffer
  void End() const;
  // Begin iven render pass using given framebuffer as an attachment
  void BeginRenderPass(const VkRenderPass &render_pass,
                       const VkFramebuffer &frame_buffer,
                       const VkExtent2D &render_area) const;
  void EndRenderPass() const;
  void BindGraphicsPipeline(const VkPipeline &pipeline) const;
  // Draw command
  void Draw(uint32_t vertex_count, uint32_t instance_count = 1,
            uint32_t first_vertex = 0, uint32_t first_instance = 0) const;

private:
  // Reference to resource this object is created with
  const CommandPool &command_pool_;
  VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif