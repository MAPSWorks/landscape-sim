#pragma once
#include <vector>
#include "vulkan_shared.h"

namespace renderer::vlk {
// Framebuffers bind swapchain images to render pass attachments
// Framebuffers are created for each image in swapchain
class Framebuffers {
public:
    Framebuffers(const VkDevice& device,  const VkRenderPass& render_pass, 
        const std::vector<VkImageView>& swapchain_iamge_views, const VkExtent2D& swapchain_extent);
    ~Framebuffers();
    Framebuffers(Framebuffers const&) = delete;
    Framebuffers operator=(Framebuffers const&) = delete;
    const std::vector<VkFramebuffer>& Get() const;
private:
    std::vector<VkFramebuffer> Create(const VkRenderPass& render_pass,
        const std::vector<VkImageView>& swapchain_iamge_views,  const VkExtent2D& swapchain_extent) const;
    // Reference to resource this object is created with
    const VkDevice& device_;
    // For each image in swapchain
    const std::vector<VkFramebuffer> framebuffers_;
};
}; // renderer vlk