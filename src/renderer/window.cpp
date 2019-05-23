#include "window.h"
#include <base/log.h>

namespace renderer {
Window::Window(const Context& context, GLFWwindow* window) :
    swapchain(context.device, context.surface.Get(), window),
    render_pass(context.device.Get(), swapchain.GetSurfaceFormat().format),
    framebuffers(context.device.Get(), render_pass.Get(),
        swapchain.GetImageViews(), swapchain.GetExtent()) {
    base::Log::Info("Renderer: windowed objects created");
}

Window::~Window() {
    base::Log::Info("Renderer: windowed objects destroying..");
}
}; // renderer