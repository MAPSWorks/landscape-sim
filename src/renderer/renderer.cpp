#include "renderer.h"
#include <vulkan/vulkan.h>
#include <base/log.h>

namespace renderer {
Renderer::Renderer(vlk::Settings settings, GLFWwindow* window) :
    context_(settings, window),
    render_pass_(context_.device.Get(), context_.swapchain.GetSurfaceFormat().format),
    pipeline_manager_(context_.device.Get(), render_pass_.Get()),
    framebuffers_(context_.device.Get(), render_pass_.Get(),
        context_.swapchain.GetImageViews(), context_.swapchain.GetExtent()),
    command_buffers_(context_.device.Get(), context_.device.GetQueue().GetFamilyIndices().graphics.value(),
        (uint32_t)framebuffers_.Get().size()),
    image_available_semaphore_(context_.device.Get()),
    render_finished_semaphore_(context_.device.Get()) {
    base::Log::Info("Renderer: renderer initialized");
}

Renderer::~Renderer() {
    base::Log::Info("Renderer: renderer shutting down...");
}

const Context& Renderer::GetContext() const {
    return context_;
}

const vlk::CommandBuffers& Renderer::GetCommandBuffers() const {
    return command_buffers_;
}

PipelineManager& Renderer::GetPipelineManager() {
    return pipeline_manager_;
}

void Renderer::RenderFrame() const {
    // -Acquire an image from the swap chain
    const auto image_index = context_.swapchain.AcquireNextImageIndex(image_available_semaphore_.Get());
    // -Execute the command buffer with that image as attachment in the framebuffer
    // There is command buffer for eatch image in swapchain
    context_.device.GetQueue().GraphicsSubmit(command_buffers_.Get().at(image_index),
        image_available_semaphore_.Get(), render_finished_semaphore_.Get());
    // -Return the image to the swap chain for presentation
    context_.device.GetQueue().Present(context_.swapchain.Get(), image_index, render_finished_semaphore_.Get());
    // TODO: recreate to frames in flight variant
    vkQueueWaitIdle(context_.device.GetQueue().GetPresent());
}

// Calling thhis function will reset whole command buffers
// Allcommand buffers are reecorded at once (for each framebuffer)
void Renderer::BeginRecordCommandBuffers() const {
    command_buffers_.Begin();
    command_buffers_.BeginRenderPass(render_pass_.Get(), framebuffers_.Get(), context_.swapchain.GetExtent());
}

void Renderer::EndRecordCommandBuffers() const {
    command_buffers_.EndRenderPass();
    command_buffers_.End();
}

void Renderer::WaitForGPUIdle() const {
    vkDeviceWaitIdle(context_.device.Get());
}
}; // renderer