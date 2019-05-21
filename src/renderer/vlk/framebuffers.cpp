#include "framebuffers.h"
#include <base/log.h>

namespace renderer::vlk {
Framebuffers::Framebuffers(const VkDevice& device, const VkRenderPass& render_pass, 
    const std::vector<VkImageView>& swapchain_iamge_views,const VkExtent2D& swapchain_extent) :
    device_(device),
    framebuffers_(Create(render_pass, swapchain_iamge_views, swapchain_extent)) {
    base::Log::Info("Renderer: all framebuffers created");
}

Framebuffers::~Framebuffers() {
    base::Log::Info("Renderer: framebuffers destroying...");
    for (auto framebuffer : framebuffers_) {
        vkDestroyFramebuffer(device_, framebuffer, nullptr);
    }
}

const std::vector<VkFramebuffer>& Framebuffers::Get() const {
    return framebuffers_;
}

std::vector<VkFramebuffer> Framebuffers::Create(const VkRenderPass& render_pass,
    const std::vector<VkImageView>& swapchain_iamge_views, const VkExtent2D& swapchain_extent) const {
    std::vector<VkFramebuffer> framebuffers(swapchain_iamge_views.size());
    uint32_t fb_index = 0;
    for (const auto& image_view : swapchain_iamge_views) {
        VkImageView attachments[] = { image_view };
        VkFramebufferCreateInfo framebuffer_create_info {};
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        // Render pass this framebuffer is compatable with
        framebuffer_create_info.renderPass = render_pass;
        framebuffer_create_info.attachmentCount = 1;
        framebuffer_create_info.pAttachments = attachments;
        framebuffer_create_info.width = swapchain_extent.width;
        framebuffer_create_info.height = swapchain_extent.height;
        framebuffer_create_info.layers = 1;
        ErrorCheck(vkCreateFramebuffer(device_, &framebuffer_create_info, nullptr, &framebuffers.at(fb_index++)));
        base::Log::Info("Renderer: framebuffer ", (fb_index-1), " created");
    }

    return framebuffers;
}
};