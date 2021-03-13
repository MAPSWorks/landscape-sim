//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/command_buffer.h"

#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/command_pool.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
CommandBuffer::CommandBuffer(const CommandPool &command_pool)
    : command_pool_(command_pool),
      command_buffer_(command_pool_.AllocateCommandBuffer(
          CommandPool::BufferLevel::kPrimary)) {

  base::Log::Info("renderer", "command buffer", "initialized");
}

VkCommandBuffer CommandBuffer::Handle() { return command_buffer_; }

void CommandBuffer::Begin(CommandBuffer::Usage usage) const {
  VkCommandBufferBeginInfo begin_info = {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = static_cast<VkCommandBufferUsageFlagBits>(usage);
  begin_info.pInheritanceInfo = nullptr;
  ErrorCheck(vkBeginCommandBuffer(command_buffer_, &begin_info));
}

void CommandBuffer::End() const {
  ErrorCheck(vkEndCommandBuffer(command_buffer_));
}

void CommandBuffer::BeginRenderPass(const VkRenderPass &render_pass,
                                    const VkFramebuffer &frame_buffer,
                                    const VkExtent2D &render_area) const {
  VkRenderPassBeginInfo render_pass_info{};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass = render_pass;
  render_pass_info.framebuffer = frame_buffer;
  render_pass_info.renderArea.offset = {0, 0};
  // Usually swapchain extent
  render_pass_info.renderArea.extent = render_area;
  // Clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, which used as load
  // operation for color attachment and depth
  std::vector<VkClearValue> clear_values(2);
  // NOTE: this order should match attachment order !
  clear_values[0].color = {{0.39F, 0.58F, 0.92F, 1.0F}};
  // The range of depths in the depth buffer is 0.0 to 1.0, by default should be
  // furthest.
  // clear_values[1].depthStencil = { 1.0f, 0 };
  render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
  render_pass_info.pClearValues = clear_values.data();
  // VK_SUBPASS_CONTENTS_INLINE is used for primary command buffers
  vkCmdBeginRenderPass(command_buffer_, &render_pass_info,
                       VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::EndRenderPass() const {
  vkCmdEndRenderPass(command_buffer_);
}

void CommandBuffer::BindGraphicsPipeline(const VkPipeline &pipeline) const {
  vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void CommandBuffer::Draw(uint32_t vertex_count, uint32_t instance_count,
                         uint32_t first_vertex, uint32_t first_instance) const {
  vkCmdDraw(command_buffer_, vertex_count, instance_count, first_vertex,
            first_instance);
}

} // namespace lsim::renderer::vlk