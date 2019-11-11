#include "window.h"
#include <base/log.h>

namespace renderer {
Window::Window(const Context& context, const vlk::MemoryAllocator& allocator) :
    context_(context) {
    CreateSwapchain(allocator);
    base::Log::Info("Renderer: windowed objects created");
}

const VkSwapchainKHR& Window::GetSwapchain() const {
    return swapchain_->Get();
}

const vlk::Swapchain& Window::GetSwapchainObject() const {
    return *swapchain_;
}

const VkRenderPass& Window::GetRenderPass() const {
    return render_pass_->Get();
}

const DepthImage& Window::GetDepthImage() const {
    return *depth_image_;
}

t::F32 Window::GetAspectRatio() const {
    return swapchain_->GetExtent().width / static_cast<t::F32>(swapchain_->GetExtent().height);
}

// Force clean-up and create new objects dependant on window format
// NOTE: not all objects depending on swapchain are in this class
void Window::RecreateSwapchain(const vlk::MemoryAllocator& allocator) {
    // Wait till all work on GPU is finished
    vkDeviceWaitIdle(context_.device.Get());
    // Clean up objects dependant on swapchain
    // Order is important!
    render_pass_.reset();
    depth_image_.reset();
    // NOTE: when recreating swapchan always should be deleted last
    swapchain_.reset();
    // Recreate
    CreateSwapchain(allocator);
}

// Create swachain renderable and objects dependant on swapchain format
void Window::CreateSwapchain(const vlk::MemoryAllocator& allocator) {
    swapchain_ = std::make_unique<vlk::Swapchain>(context_.device, context_.surface.Get(), context_.window_glfw);
    depth_image_ = std::make_unique<DepthImage>(context_.device.Get(), allocator, swapchain_->GetExtent());
    render_pass_ = std::make_unique<vlk::RenderPass>(context_.device.Get(),
        swapchain_->GetSurfaceFormat().format, depth_image_->GetFormat());
}
}; // renderer