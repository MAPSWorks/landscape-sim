#pragma once
#include <renderer/context.h>
#include <renderer/vlk/render_pass.h>
#include <renderer/vlk/framebuffers.h>
#include <renderer/vlk/graphics_pipeline.h>
#include <renderer/vlk/command_buffers.h>
#include <renderer/vlk/semaphore.h>

namespace graphics::objects {
class Triangle {
public:
    Triangle(const renderer::Context& context);
    ~Triangle();
    void RenderFrame() const;
private:
    // Reference to resources this object was created with
    const renderer::Context& context_;
    const renderer::vlk::RenderPass render_pass_;
    const renderer::vlk::Framebuffers framebuffers_;
    const renderer::vlk::GraphicsPipeline graphics_pipeline_;
    const renderer::vlk::CommandBuffers command_buffers_;;
    const renderer::vlk::Semaphore image_available_semaphore_;
    const renderer::vlk::Semaphore render_finished_semaphore_;
};
}; //graphics objects