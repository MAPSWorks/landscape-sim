#include "command_buffer.h"
#include <base/log.h>

namespace renderer::vlk {
CommandBuffer::CommandBuffer(const CommandPool& command_pool) :
    command_buffer_(command_pool.AllocateCommandPrimaryBuffer()) {
    base::Log::Info("Renderer: primary command buffer allocated");
}

const VkCommandBuffer& CommandBuffer::Get() const {
    return command_buffer_;
}

// Recording
void CommandBuffer::Begin() const {
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // Specifies that each recording of the command buffer will only be submitted once, and the 
    // command buffer will be reset and recorded again between each submission.
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    ErrorCheck(vkBeginCommandBuffer(command_buffer_, &begin_info));
}

void CommandBuffer::End() const {
    ErrorCheck(vkEndCommandBuffer(command_buffer_));
}

void CommandBuffer::BeginRenderPass(const VkRenderPass& render_pass, const VkFramebuffer& frame_buffer, 
    const VkExtent2D& render_area_extent) const {
    VkRenderPassBeginInfo render_pass_Info{};
    render_pass_Info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_Info.renderPass = render_pass;
    render_pass_Info.framebuffer = frame_buffer;
    render_pass_Info.renderArea.offset = { 0, 0 };
    // Usually swapchain extent
    render_pass_Info.renderArea.extent = render_area_extent;
    // Clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, which used as load operation for color attachment.
    VkClearValue clear_color = { 0.1f, 0.1f, 0.1f, 1.0f };
    render_pass_Info.clearValueCount = 1;
    render_pass_Info.pClearValues = &clear_color;
    vkCmdBeginRenderPass(command_buffer_, &render_pass_Info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::EndRenderPass() const {
    vkCmdEndRenderPass(command_buffer_);
}

void CommandBuffer::BindGraphicsPipeline(const VkPipeline& pipeline) const {
    vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void CommandBuffer::BindBuffer(const VkBuffer& buffer, uint32_t binding_index) const {
    VkBuffer vertex_buffers[] = { buffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(command_buffer_, binding_index, 1, vertex_buffers, offsets);
}

void CommandBuffer::Draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, 
    uint32_t first_instance) const {
    vkCmdDraw(command_buffer_, vertex_count, instance_count, first_vertex, first_instance);
}
};