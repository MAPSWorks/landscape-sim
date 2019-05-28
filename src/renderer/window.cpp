#include "window.h"
#include <base/log.h>

namespace renderer {
    Window::Window(const Context& context) : 
    context_(context) {
    CreateSwapchain();
    base::Log::Info("Renderer: windowed objects created");
}

Window::~Window() {
    base::Log::Info("Renderer: windowed objects destroying..");
}

const VkSwapchainKHR& Window::GetSwapchain() const {
    return swapchain_.get()->Get();
}

const vlk::Swapchain& Window::GetSwapchainObject() const {
    return *swapchain_.get();
}

const VkRenderPass& Window::GetRenderPass() const {
    return render_pass_.get()->Get();
}

const std::vector<VkFramebuffer>& Window::GetFramebuffers() const {
    return framebuffers_.get()->Get();
}

// Force clean-up and create new objects dependant on window format
// NOTE: not all objects depending on swapchain are in this class
void Window::RecreateSwapchain() {
    // Wait till all work on GPU is finished
    vkDeviceWaitIdle(context_.device.Get());
    // Clean up objects dependant on swapchain
    // Order is important!
    framebuffers_.reset();
    render_pass_.reset();
    // NOTE: when recreating swapchan always should be deleted last
    swapchain_.reset();
    // Recreate
    CreateSwapchain();
}

// Create swachain object and objects dependant on swapchain format
void Window::CreateSwapchain() {
    swapchain_ = std::make_unique<vlk::Swapchain>(context_.device, context_.surface.Get(), context_.window_glfw);
    render_pass_ = std::make_unique<vlk::RenderPass>(context_.device.Get(),
        GetSwapchainObject().GetSurfaceFormat().format);
    framebuffers_ = std::make_unique<vlk::Framebuffers>(context_.device.Get(), GetRenderPass(),
        GetSwapchainObject().GetImageViews(), GetSwapchainObject().GetExtent());
}
}; // renderer