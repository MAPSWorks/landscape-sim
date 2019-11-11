#pragma once
#include "vulkan_shared.h"

namespace renderer::vlk {
class RenderPass {
public:
    RenderPass(const VkDevice& device, const VkFormat& swapchain_format, const VkFormat& depth_format);
    ~RenderPass();
    RenderPass(RenderPass const&) = delete;
    RenderPass operator=(RenderPass const&) = delete;
    const VkRenderPass& Get() const;
private:
    VkRenderPass Create(const VkFormat& swapchain_format, const VkFormat& depth_format) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkRenderPass render_pass_ = VK_NULL_HANDLE;
};
}; // renderer vlk