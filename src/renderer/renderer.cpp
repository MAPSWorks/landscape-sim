#include "renderer.h"
#include <base/log.h>

namespace renderer {
Renderer::Renderer(vlk::Settings settings, GLFWwindow* window) :
    context_(settings, window),
    window_(context_),
    pipeline_manager_(context_.device.Get()),
    command_buffers_(context_.device.Get(), context_.device.GetQueue().GetFamilyIndices().graphics.value(),
        (uint32_t)window_.GetFramebuffers().size()),
    sync_manager_(context_.device.Get(), (uint32_t)window_.GetFramebuffers().size()) {
    base::Log::Info("Renderer: renderer initialized");
}

Renderer::~Renderer() {
    base::Log::Info("Renderer: renderer shutting down...");
}

const Context& Renderer::GetContext() const {
    return context_;
}

const Window& Renderer::GetWindow() const {
    return window_;
}

const vlk::CommandBuffers& Renderer::GetCommandBuffers() const {
    return command_buffers_;
}

PipelineManager& Renderer::GetPipelineManager() {
    return pipeline_manager_;
}

uint32_t Renderer::BeginFrame() {
    // Frames on GPU are processed in paralel, therefore we need to synchronize with cpu
    sync_manager_.WaitForInFlightFence();
    // -Acquire an image from the swap chain
    const auto [image_index, image_result] =
        window_.GetSwapchainObject().AcquireNextImageIndex(sync_manager_.GetImageAvailableSemaphore());
    return image_index;
}

void Renderer::EndFrame(uint32_t image_index) {
    // -Execute the command buffer with that image as attachment in the framebuffer
    // There is command buffer for eatch image in swapchain
    context_.device.GetQueue().GraphicsSubmit(command_buffers_.Get(image_index),
        sync_manager_.GetImageAvailableSemaphore(), sync_manager_.GetRenderFinishedSemaphore(), sync_manager_.GetInFlightFence());
    // -Return the image to the swap chain for presentation
    context_.device.GetQueue().Present(window_.GetSwapchain(), image_index, sync_manager_.GetRenderFinishedSemaphore());
    sync_manager_.UpdateFrameInFlight();
}

// Calling thhis function will reset whole command buffer
void Renderer::BeginRecordCommandBuffer(uint32_t buffer_index) const {
    command_buffers_.Begin(buffer_index);
    command_buffers_.BeginRenderPass(buffer_index, window_.GetRenderPass(), window_.GetFramebuffers(), window_.GetSwapchainObject().GetExtent());
}

void Renderer::EndRecordCommandBuffer(uint32_t buffer_index) const {
    command_buffers_.EndRenderPass(buffer_index);
    command_buffers_.End(buffer_index);
}

void Renderer::WaitForGPUIdle() const {
    sync_manager_.DeviceWaitIdle();
}

void Renderer::Resize() {
    window_.RecreateSwapchain();
    pipeline_manager_.RecreatePipelines(window_.GetRenderPass(), window_.GetSwapchainObject().GetExtent());
}
}; // renderer