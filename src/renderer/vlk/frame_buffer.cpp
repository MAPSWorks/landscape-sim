#include "frame_buffer.h"
#include <base/log.h>


namespace renderer::vlk {
FrameBuffer::FrameBuffer(const VkDevice& device) :
    device_(device) {
    base::Log::Info("Renderer: framebuffer initialized");
}

FrameBuffer::~FrameBuffer() {
    base::Log::Info("Renderer: framebuffer destroying...");
    Destroy();
}

const VkFramebuffer& FrameBuffer::Get() const {
    return frame_buffer_;
}

void FrameBuffer::Create(const VkRenderPass& render_pass, const VkImageView& swapchain_iamge_view, 
    const VkExtent2D& swapchain_extent) {
    VkImageView attachments[] = { swapchain_iamge_view };
    VkFramebufferCreateInfo framebuffer_create_info{};
    framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    // Render pass this framebuffer is compatable with
    framebuffer_create_info.renderPass = render_pass;
    framebuffer_create_info.attachmentCount = 1;
    framebuffer_create_info.pAttachments = attachments;
    framebuffer_create_info.width = swapchain_extent.width;
    framebuffer_create_info.height = swapchain_extent.height;
    framebuffer_create_info.layers = 1;
    ErrorCheck(vkCreateFramebuffer(device_, &framebuffer_create_info, nullptr, &frame_buffer_));
}

// Can be called explicitly
void FrameBuffer::Destroy() {
    if (frame_buffer_ != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(device_, frame_buffer_, nullptr);
    }
    frame_buffer_ = VK_NULL_HANDLE;
}
}; // renderer vlk