#include "command_buffer.h"
#include <base/log.h>

namespace renderer::vlk {
CommandBuffer::CommandBuffer(const CommandPool& command_pool) :
    command_pool_(command_pool),
    command_buffer_(command_pool_.AllocateCommandPrimaryBuffer()) {
    base::Log::Info("Renderer: primary command buffer allocated");
}

const VkCommandBuffer& CommandBuffer::Get() const {
    return command_buffer_;
}

// Freeing is not mandatory because command buffers get destroyed together with comand pool
void CommandBuffer::Free(const VkDevice& device) const {
    vkFreeCommandBuffers(device, command_pool_.Get(), 1, &command_buffer_);
}

// Recording
void CommandBuffer::Begin(Usage usage) const {
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = static_cast<VkCommandBufferUsageFlagBits>(usage);
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
    // Clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, which used as load operation for color attachment and depth
    std::vector<VkClearValue> clear_values(2);
    // NOTE: this order should match attachment order !
    clear_values[0].color = { 0.39f, 0.58f, 0.92f, 1.0f };
    // The range of depths in the depth buffer is 0.0 to 1.0, by default should be furthest.
    clear_values[1].depthStencil = { 1.0f, 0 };
    render_pass_Info.clearValueCount = static_cast<t::U32>(clear_values.size());
    render_pass_Info.pClearValues = clear_values.data();
    vkCmdBeginRenderPass(command_buffer_, &render_pass_Info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::EndRenderPass() const {
    vkCmdEndRenderPass(command_buffer_);
}

void CommandBuffer::BindGraphicsPipeline(const VkPipeline& pipeline) const {
    vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void CommandBuffer::BindVertexBuffer(const VkBuffer& buffer, t::U32 binding_index) const {
    VkBuffer vertex_buffers[] = { buffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(command_buffer_, binding_index, 1, vertex_buffers, offsets);
}

void CommandBuffer::BindIndexBuffer32(const VkBuffer& buffer) const {
    VkDeviceSize offset = 0;
    vkCmdBindIndexBuffer(command_buffer_, buffer, offset, VK_INDEX_TYPE_UINT32);
}

void CommandBuffer::Draw(t::U32 vertex_count, t::U32 instance_count, t::U32 first_vertex,
    uint32_t first_instance) const {
    vkCmdDraw(command_buffer_, vertex_count, instance_count, first_vertex, first_instance);
}

void CommandBuffer::DrawIndexed(t::U32 index_count, t::U32 instance_count, t::U32 first_index,
    uint32_t vertex_offset, uint32_t first_instance) const {
    vkCmdDrawIndexed(command_buffer_, index_count, instance_count, first_index, vertex_offset, first_instance);
}

void CommandBuffer::CopyBuffer(const VkBuffer& src_buffer, const VkBuffer& dst_buffer,
    VkDeviceSize size, VkDeviceSize src_offset, VkDeviceSize dst_offset) const {
    VkBufferCopy copy_region {};
    copy_region.srcOffset = src_offset;
    copy_region.dstOffset = dst_offset;
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer_, src_buffer, dst_buffer, 1, &copy_region);
}

// Bound DescriptorSets stay active as long as the PipelineLayout for that binding slot is matching.
void CommandBuffer::BindGraphicsDescriptorSet(const VkDescriptorSet& descriptor_set, const VkPipelineLayout& layout, t::U32 first_set_at_index) const {
    // Currently one descriptor set is binded
    // TODO: If this changes add 's' to function name
    t::U32 descriptor_set_count = 1;
    vkCmdBindDescriptorSets(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, first_set_at_index, descriptor_set_count, &descriptor_set, 0, nullptr);
}

void CommandBuffer::PipelineBarrier(VkPipelineStageFlags src_stagemask, VkPipelineStageFlags dst_stagemask,
    VkDependencyFlags dependancy_flags, t::U32 memory_barrier_count, const VkMemoryBarrier* memory_barriers, 
    t::U32 buffer_memory_barrier_count, const VkBufferMemoryBarrier* buffer_memory_barriers,
    t::U32 image_memory_barrier_count, const VkImageMemoryBarrier* image_memory_barriers) const {
    vkCmdPipelineBarrier(command_buffer_, src_stagemask, dst_stagemask, dependancy_flags,
        memory_barrier_count, memory_barriers, buffer_memory_barrier_count, buffer_memory_barriers,
        image_memory_barrier_count, image_memory_barriers);
}

void CommandBuffer::CopyBufferToImage2D(const VkBuffer& buffer, const VkImage& image, const t::Size32& dimensions, VkImageLayout layout) const {
    VkBufferImageCopy buf_img_copy {};
    buf_img_copy.bufferOffset = 0;
    buf_img_copy.bufferRowLength = 0;
    buf_img_copy.bufferImageHeight = 0;
    buf_img_copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    buf_img_copy.imageSubresource.mipLevel = 0;
    buf_img_copy.imageSubresource.baseArrayLayer = 0;
    buf_img_copy.imageSubresource.layerCount = 1;
    buf_img_copy.imageOffset = { 0, 0, 0 };
    buf_img_copy.imageExtent = { dimensions.width, dimensions.height, 1 };
    vkCmdCopyBufferToImage(command_buffer_, buffer, image, layout, 1, &buf_img_copy);
}
};