#include "command_buffers.h"
#include <base/log.h>

namespace renderer::vlk {
CommandBuffers::CommandBuffers(const VkDevice& device, uint32_t queue_family_index, uint32_t count) :
    device_(device),
    command_pool_(CreateCommandPool(queue_family_index)),
    command_buffers_(AllocateCommandBuffers(count)) {
    base::Log::Info("Renderer: command pool created and '",count,"'command buffers allocated");
}

CommandBuffers::~CommandBuffers() {
    base::Log::Info("Renderer: command pool (and implicitly command buffers) destroying...");
    vkDestroyCommandPool(device_, command_pool_, nullptr);
    // Command buffers are destroyed together with pool
}

const std::vector<VkCommandBuffer>& CommandBuffers::Get() const {
    return command_buffers_;
}

const VkCommandBuffer& CommandBuffers::Get(uint32_t index) const {
    return command_buffers_.at(index);
}

// Record individual command buffer
void CommandBuffers::Begin(uint32_t index) const {
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    ErrorCheck(vkBeginCommandBuffer(Get(index), &begin_info));
}

void CommandBuffers::End(uint32_t index) const {
    ErrorCheck(vkEndCommandBuffer(Get(index)));
}

// No error checks for performance
void CommandBuffers::BeginRenderPass(uint32_t index, const VkRenderPass& render_pass,
    const std::vector<VkFramebuffer>& frame_buffers, const VkExtent2D& render_area_extent) const {
    VkRenderPassBeginInfo render_Pass_Info {};
    render_Pass_Info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_Pass_Info.renderPass = render_pass;
    render_Pass_Info.framebuffer = frame_buffers.at(index);
    render_Pass_Info.renderArea.offset = { 0, 0 };
    // Usually swapchain extent
    render_Pass_Info.renderArea.extent = render_area_extent;
    // Clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, which used as load operation for color attachment.
    VkClearValue clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
    render_Pass_Info.clearValueCount = 1;
    render_Pass_Info.pClearValues = &clear_color;
    vkCmdBeginRenderPass(Get(index), &render_Pass_Info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffers::EndRenderPass(uint32_t index) const {
    vkCmdEndRenderPass(Get(index));
}

void CommandBuffers::BindGraphicsPipeline(uint32_t index, const VkPipeline& pipeline) const {
    vkCmdBindPipeline(Get(index), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void CommandBuffers::CommandBuffers::Draw(uint32_t index, uint32_t vertex_count, uint32_t instance_count,
    uint32_t first_vertex, uint32_t first_instance) const {
    vkCmdDraw(Get(index), vertex_count, instance_count, first_vertex, first_instance);
}

// family_index - in this case should be graphics queueu family
VkCommandPool CommandBuffers::CreateCommandPool(uint32_t family_index) const { 
    VkCommandPoolCreateInfo pool_create_info {};
    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.queueFamilyIndex = family_index;
    // Command buffer can be re recorded
    bool is_rerecordable = true;
    if (is_rerecordable) {
        pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    }
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