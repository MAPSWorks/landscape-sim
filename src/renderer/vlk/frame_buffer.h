#pragma once
#include "vulkan_shared.h"

// No RAII since this class because this is for frame resource where
// framebuffer is recreated every frame.
// We dont create it in constructor but only when needed using Create function
// Destruction is performed using Destroy function and also implicitly in destructor
// if framebuffer is currently initialized
namespace renderer::vlk {
class FrameBuffer {
public:
    FrameBuffer(const VkDevice& device);
    ~FrameBuffer();
    FrameBuffer(FrameBuffer const&) = delete;
    FrameBuffer operator=(FrameBuffer const&) = delete;
    const VkFramebuffer& Get() const;
    // Create frame buffer for a given combination of inputs
    void Create(const VkRenderPass& render_pass, const VkImageView& swapchain_iamge_view,
        const VkExtent2D& swapchain_extent);
    // Can be called explicitly
    void Destroy();
private:
    // Reference to resource this object is created with
    const VkDevice& device_;
    VkFramebuffer frame_buffer_ = VK_NULL_HANDLE;
};
}; // renderer vlk