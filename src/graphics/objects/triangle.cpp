#include "triangle.h"
#include <util/log.h>
//#include <renderer/vlk/shader_module.h>

namespace graphics::objects {
    Triangle::Triangle(const renderer::Context& context) :
        context_(context),
        render_pass_(context_.device.Get(), context.swapchain.GetSurfaceFormat().format),
        framebuffers_(context_.device.Get(), render_pass_.Get(),
            context_.swapchain.GetImageViews(), context_.swapchain.GetExtent()),
        graphics_pipeline_(context_.device.Get(), render_pass_.Get(),
            renderer::vlk::GraphicsPipeline::CreateParams{
            // Pipeline name
            "Triangle pipeline",
            // Programmable stage
            {
                    // Shader stages
                {"shaders/vert.spv", renderer::vlk::GraphicsPipeline::ShaderStage::kVertex },
                {"shaders/frag.spv", renderer::vlk::GraphicsPipeline::ShaderStage::kFragment },
            },
            // Fixed function state
            // Primitive assembly
            {
                // Primitive topology
                renderer::vlk::GraphicsPipeline::PrimitiveTopology::kTriangleList
            },
            // Viewport and scisors
            {
                // Viewport size
                context_.swapchain.GetExtent().width, context_.swapchain.GetExtent().height,
            },
            // Rasterization
            {
                renderer::vlk::GraphicsPipeline::PolygonMode::kFill,
                renderer::vlk::GraphicsPipeline::CullMode::kNone
            },
            // Multisample
            {
                false
            },
            // Color blending
            {
                false
            },
            // Pipeline layout
            {
                0
            }
            }),
        command_buffers_(context_.device.Get(), context_.device.GetQueue().GetFamilyIndices().graphics.value(),
            (uint32_t)framebuffers_.Get().size()),
        image_available_semaphore_(context_.device.Get()),
        render_finished_semaphore_(context_.device.Get()) {
    // Record command buffers
    command_buffers_.Begin();
    command_buffers_.BeginRenderPass(render_pass_.Get(), framebuffers_.Get(), context_.swapchain.GetExtent());
    command_buffers_.BindGraphicsPipeline(graphics_pipeline_.Get());
    command_buffers_.Draw(3, 1, 0, 0);
    command_buffers_.EndRenderPass();
    command_buffers_.End();
    util::Log::Info("Graphics objects: command buffers recorded");
    util::Log::Info("Graphics objects: triangle created");

}

Triangle::~Triangle() {
    util::Log::Info("Graphics objects: triangle destroying...");
}

void Triangle::RenderFrame() const {
    // -Record command buffer
    // -Acquire an image from the swap chain
    const auto image_index = context_.swapchain.AcquireNextImageIndex(image_available_semaphore_.Get());
    // -Execute the command buffer with that image as attachment in the framebuffer
    // There is command buffer for eatch image in swapchain
    context_.device.GetQueue().GraphicsSubmit(command_buffers_.Get().at(image_index), 
        image_available_semaphore_.Get(), render_finished_semaphore_.Get());
    // -Return the image to the swap chain for presentation
    context_.device.GetQueue().Present(context_.swapchain.Get(), image_index, render_finished_semaphore_.Get());
}
}; //graphics objects