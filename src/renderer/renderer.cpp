#include "renderer.h"
#include <base/log.h>
#include "types.h"

namespace renderer {
Renderer::Renderer(const base::JSONLoader& setting_loader, GLFWwindow* window) :
    kFramesInFlight(setting_loader.Get().at("renderer").at("framesInFlight").get<t::U32>()),
    context_(setting_loader, window),
    memory_allocator_(context_.device),
    window_(context_, memory_allocator_),
    frame_manager_(context_.device.Get(),
        context_.device.GetQueue().GetFamilyIndices().graphics.value(),
        kFramesInFlight),
    one_time_commands_(context_.device),
    shader_resources_(context_.device.Get(), GetMemoryAllocator(), kFramesInFlight),
    pipeline_manager_(context_.device.Get()) {
    base::Log::Info("Renderer: renderer initialized");
}

const Context& Renderer::GetContext() const {
    return context_;
}

const Window& Renderer::GetWindow() const {
    return window_;
}

ShaderResources& Renderer::GetShaderResources() {
    return shader_resources_;
}

PipelineManager& Renderer::GetPipelineManager() {
    return pipeline_manager_;
}

void Renderer::WaitForGPUIdle() const {
    vkDeviceWaitIdle(context_.device.Get());
}

void Renderer::Resize() {
    // Maybe it is safer to set bolean resize variable here
    // and do the swapchain recreation in render function
    // because what if swapchain aquired image dissynchronizes with
    // sync_manager internal counter upon resizing?
    window_.RecreateSwapchain(memory_allocator_);
    pipeline_manager_.RecreatePipelines(window_.GetRenderPass(), window_.GetSwapchainObject().GetExtent());
}

void Renderer::BeginRecordCurrentCommandBuffer() {
    const FrameResource& current_frame = frame_manager_.GetCurrentFrameResource();
    current_frame.command_buffer.Begin(vlk::CommandBuffer::Usage::kOneTimeSubmit);
    current_frame.command_buffer.BeginRenderPass(window_.GetRenderPass(), current_frame.frame_buffer.Get(), 
        window_.GetSwapchainObject().GetExtent());
}

void Renderer::EndRecordCurrentCommandBuffer() {
    const FrameResource& current_frame = frame_manager_.GetCurrentFrameResource();
    current_frame.command_buffer.EndRenderPass();
    current_frame.command_buffer.End();
}

const vlk::CommandBuffer& Renderer::GetCurrentCommandBuffer() {
    return frame_manager_.GetCurrentFrameResource().command_buffer;
}

FrameManager::FrameId Renderer::FrameBegin() {
    // Get currently processed frame 
    // (processed on CPU, not GPU. On GPU some other frame is processed right now)
    FrameResource& current_frame = frame_manager_.GetCurrentFrameResource();
    // Make sure drawing of this frame's resources is already completed
    current_frame.drawing_finished_fence.WaitFor();
    current_frame.drawing_finished_fence.Reset();
    // Since we recreate framebuffer for every frame destroy previously used now
    current_frame.frame_buffer.Destroy();
    // Store swapchain image index that is next to be presented and use it in subsequent calls
    const auto image_index = window_.GetSwapchainObject().AcquireNextImageIndex(current_frame.image_acquired_semaphore.Get());
    current_frame.image_index = image_index;
    // Create framebuffer for this frame
    current_frame.frame_buffer.Create(window_.GetRenderPass(), 
        window_.GetSwapchainObject().GetImageViews().at(current_frame.image_index).Get(), 
        window_.GetSwapchainObject().GetExtent());
    return frame_manager_.GetCurrentFrameIndex();
}

void Renderer::FrameEnd() {
    const FrameResource& current_frame = frame_manager_.GetCurrentFrameResource();
    // Send to GPU
    context_.device.GetQueue().SubmitGraphics(current_frame.command_buffer.Get(),
        current_frame.image_acquired_semaphore.Get(),
        current_frame.ready_to_present_semaphore.Get(),
        current_frame.drawing_finished_fence.Get());
    context_.device.GetQueue().Present(window_.GetSwapchain(), current_frame.image_index, 
        current_frame.ready_to_present_semaphore.Get());
    // Switch to other frame resource to process next
    frame_manager_.Update();
}

const vlk::MemoryAllocator& Renderer::GetMemoryAllocator() const {
    return memory_allocator_;
}

const FrameManager& Renderer::GetFrameManager() const {
    return frame_manager_;
}

const OneTimeCommands& Renderer::GetOneTimeCommands() const {
    return one_time_commands_;
}
}; // renderer