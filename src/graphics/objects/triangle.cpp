#include "triangle.h"
#include <util/log.h>
//#include <renderer/vlk/shader_module.h>

namespace graphics::objects {
    Triangle::Triangle(const renderer::Context& context) :
        context_(context),
        graphics_pipeline_(context_.device.Get(), context.swapchain.GetSurfaceFormat().format,
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
            }) {
    util::Log::Info("Graphics objects: triangle created");
}

Triangle::~Triangle() {
    util::Log::Info("Graphics objects: triangle destroying...");
}

void Triangle::Draw() const {
    // Record command buffer

    // Submit command buffer
    // vkQueueSubmit(queue, 1, &submitInfo, NULL);

    // Present to screen ?
    // vkQueuePresentKHR(queue, &presentInfo);
}
}; //graphics