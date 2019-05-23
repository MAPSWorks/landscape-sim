#pragma once
#include <GLFW/glfw3.h>
#include "vlk/types.h"
#include "vlk/render_pass.h"
#include "vlk/framebuffers.h"
#include "vlk/command_buffers.h"
#include "vlk/semaphore.h"
#include "context.h"
#include "pipeline_manager.h"

namespace renderer {
// General rendering class used to set-up means to render and render data in
// it's pure form (without knowing details of what is being rendered)
class Renderer {
public:
    Renderer(vlk::Settings settings, GLFWwindow* window);
    ~Renderer();
    const Context& GetContext() const;
    const vlk::CommandBuffers& GetCommandBuffers() const;
    PipelineManager& GetPipelineManager();
    void RenderFrame() const;
    void BeginRecordCommandBuffers() const;
    void EndRecordCommandBuffers() const;
    // Used to ensure nothing is currently in use on GPU
    void WaitForGPUIdle() const;
private:
    const Context context_;
    const vlk::RenderPass render_pass_;
    PipelineManager pipeline_manager_;
    const vlk::Framebuffers framebuffers_;
    const vlk::CommandBuffers command_buffers_;;
    const vlk::Semaphore image_available_semaphore_;
    const vlk::Semaphore render_finished_semaphore_;

};
}; // renderer