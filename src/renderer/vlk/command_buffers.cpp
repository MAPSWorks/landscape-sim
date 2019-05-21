#include "command_buffers.h"
#include <base/log.h>

namespace renderer::vlk {
CommandBuffers::CommandBuffers(const VkDevice& device, uint32_t queue_family_index, uint32_t count) :
    device_(device),
    command_pool_(CreateCommandPool(queue_family_index)),
    command_buffers_(AllocateCommandBuffers(count)) {
    base::Log::Info("Renderer: command pool created and command buffers allocated");
}

CommandBuffers::~CommandBuffers() {
    base::Log::Info("Renderer: command pool (and implicitly command buffers) destroying...");
    vkDestroyCommandPool(device_, command_pool_, nullptr);
    // Command buffers are destroyed together with pool
}

const std::vector<VkCommandBuffer>& CommandBuffers::Get() const {
    return command_buffers_;
}

// These functions apply to all command buffers at once because
// all buffers will have the same commands
// Calling this function resets (deletes contents) command buffer
void CommandBuffers::Begin() const {
    for (const auto& command_buffer : command_buffers_) {
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        ErrorCheck(vkBeginCommandBuffer(command_buffer, &begin_info));
    }
}

void CommandBuffers::End() const {
    for (const auto& command_buffer : command_buffers_) {
        ErrorCheck(vkEndCommandBuffer(command_buffer));
    }
}

// No error checks for performance
void CommandBuffers::BeginRenderPass(const VkRenderPass& render_pass, 
    const std::vector<VkFramebuffer>& frame_buffers, const VkExtent2D& render_area_extent) const {
    for (size_t i = 0; i < command_buffers_.size(); i++) {
        VkRenderPassBeginInfo render_Pass_Info{};
        render_Pass_Info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_Pass_Info.renderPass = render_pass;
        render_Pass_Info.framebuffer = frame_buffers[i];
        render_Pass_Info.renderArea.offset = { 0, 0 };
        // Usually swapchain extent
        render_Pass_Info.renderArea.extent = render_area_extent;
        // Clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, which used as load operation for color attachment.
        VkClearValue clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
        render_Pass_Info.clearValueCount = 1;
        render_Pass_Info.pClearValues = &clear_color;
        vkCmdBeginRenderPass(command_buffers_[i], &render_Pass_Info, VK_SUBPASS_CONTENTS_INLINE);
    }
}

void CommandBuffers::EndRenderPass() const {
    for (const auto& command_buffer : command_buffers_) {
        vkCmdEndRenderPass(command_buffer);
    }
}

void CommandBuffers::BindGraphicsPipeline(const VkPipeline& pipeline) const {
    for (const auto& command_buffer : command_buffers_) {
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }
}

void CommandBuffers::CommandBuffers::Draw(uint32_t vertex_count, uint32_t instance_count, 
    uint32_t first_vertex, uint32_t first_instance) const {
    for (const auto& command_buffer : command_buffers_) {
        vkCmdDraw(command_buffer, vertex_count, instance_count, first_vertex, first_instance);
    }
}

// family_index - in this case should be graphics queueu family
VkCommandPool CommandBuffers::CreateCommandPool(uint32_t family_index) const { 
    VkCommandPoolCreateInfo pool_create_info {};
    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.queueFamilyIndex = family_index;
    base::Log::Info("Renderer: command pool queue family index set to '", family_index ,"'");
    VkCommandPool command_pool;
    ErrorCheck(vkCreateCommandPool(device_, &pool_create_info, nullptr, &command_pool));
    return command_pool;
}

// count - number of buffer should be set to number of framebuffers
std::vector<VkCommandBuffer> CommandBuffers::AllocateCommandBuffers(uint32_t count) const {
    // Will have to record command buffer for every image in the swap chain
    std::vector<VkCommandBuffer> command_buffers(count);
    VkCommandBufferAllocateInfo command_buffer_allocinfo {};
    command_buffer_allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocinfo.commandPool = command_pool_;
    command_buffer_allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocinfo.commandBufferCount = static_cast<uint32_t>(command_buffers.size());
    ErrorCheck(vkAllocateCommandBuffers(device_, &command_buffer_allocinfo, command_buffers.data()));
    return command_buffers;
}
}; // renderer vlk