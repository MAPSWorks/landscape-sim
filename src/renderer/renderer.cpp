#include "renderer.h"
#include <vulkan/vulkan.h>
#include <base/log.h>

namespace renderer {
Renderer::Renderer(vlk::Settings settings, GLFWwindow* window) :
    context_(settings, window) {
    base::Log::Info("Renderer: renderer initialized");
}

Renderer::~Renderer() {
    base::Log::Info("Renderer: renderer shutting down...");
}

const Context& Renderer::GetContext() const {
    return context_;
}

void Renderer::RenderFrame() const {

}

void Renderer::WaitForGPUIdle() const {
    vkDeviceWaitIdle(context_.device.Get());
}
}; // renderer