#pragma once
#include <GLFW/glfw3.h>
#include <base/types.h>
#include "vlk/types.h"
#include "vlk/command_buffers.h"
#include "context.h"
#include "window.h"
#include "pipeline_manager.h"
#include "sync_manager.h"

namespace renderer {
// General rendering class used to set-up means to render and render data in
// it's pure form (without knowing details of what is being rendered)
class Renderer {
public:
    Renderer(vlk::Settings settings, GLFWwindow* window);
    ~Renderer();
    const Context& GetContext() const;
    const Window& GetWindow() const;
    const vlk::CommandBuffers& GetCommandBuffers() const;
    PipelineManager& GetPipelineManager();
    // Begine rendering and return current index of swapchain image
    uint32_t BeginFrame();
    // Submit graphics and presentation queues with command buffer at index
    void EndFrame(uint32_t image_index);
    void BeginRecordCommandBuffer(uint32_t buffer_index) const;
    void EndRecordCommandBuffer(uint32_t buffer_index) const;
    // Used to ensure nothing is currently in use on GPU
    void WaitForGPUIdle() const;
    // Window is resized, new size is passed through parameter
    void Resize();
private:
    // Constant throughout the life of the renderer
    const Context context_;
    // Changed upon window resize (dependant on window siize)
    Window window_;
    PipelineManager pipeline_manager_;
    const vlk::CommandBuffers command_buffers_;;
    SyncManager sync_manager_;
};
}; // renderer