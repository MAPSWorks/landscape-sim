#include "renderer.h"
#include <base/log.h>

namespace renderer {
Renderer::Renderer(vlk::Settings settings, GLFWwindow* window) :
    context_(settings, window),
    window_(context_, window),
    pipeline_manager_(context_.device.Get(), window_.render_pass.Get()),
    command_buffers_(context_.device.Get(), context_.device.GetQueue().GetFamilyIndices().graphics.value(),
        (uint32_t)window_.framebuffers.Get().size()),
    sync_manager_(context_.device.Get()) {
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

void Renderer::RenderFrame() {
    // Frames on GPU are processed in paralel, there fore we need to synchronize with cpu
    sync_manager_.WaitForInFlightFence();
    // -Acquire an image from the swap chain
    const auto image_index = window_.swapchain.AcquireNextImageIndex(sync_manager_.GetImageAvailableSemaphore());
    // -Execute the command buffer with that image as attachment in the framebuffer
    // There is command buffer for eatch image in swapchain
    context_.device.GetQueue().GraphicsSubmit(command_buffers_.Get().at(image_index),
        sync_manager_.GetImageAvailableSemaphore(), sync_manager_.GetRenderFinishedSemaphore(), sync_manager_.GetInFlightFence());
    // -Return the image to the swap chain for presentation
    context_.device.GetQueue().Present(window_.swapchain.Get(), image_index, sync_manager_.GetRenderFinishedSemaphore());
    sync_manager_.UpdateFrameInFlight();
}

// Calling thhis function will reset whole command buffers
// Allcommand buffers are reecorded at once (for each framebuffer)
void Renderer::BeginRecordCommandBuffers() const {
    command_buffers_.Begin();
    command_buffers_.BeginRenderPass(window_.render_pass.Get(), window_.framebuffers.Get(), window_.swapchain.GetExtent());
}

void Renderer::EndRecordCommandBuffers() const {
    command_buffers_.EndRenderPass();
    command_buffers_.End();
}

void Renderer::WaitForGPUIdle() const {
    sync_manager_.DeviceWaitIdle();
}
}; // renderer