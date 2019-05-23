#include "triangle.h"
#include <base/log.h>

namespace object {
Triangle::Triangle(renderer::Renderer& renderer) :
    renderer_(renderer),
    pipeline_description_(renderer::vlk::GraphicsPipeline::CreateParams{
    // Pipeline descriptive name, should differ for different pipelines
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
        // TODO:
        800, 600
       /* renderer_.GetContext().swapchain.GetExtent().width, renderer_.GetContext().swapchain.GetExtent().height,*/
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
    pipeline_id_(renderer_.GetPipelineManager().AddGraphicsPipeline(pipeline_description_))  {
    base::Log::Info("Objects: triangle created");
}

Triangle::~Triangle() {
    base::Log::Info("Objects: triangle destroying...");
}

// Command buffer and render pass is already started
void Triangle::RecordToCommandBuffer() const {
    // For rendering triangle
    // Assuming we have nnly one set of command buffers
    renderer_.GetCommandBuffers().BindGraphicsPipeline(renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_));
    renderer_.GetCommandBuffers().Draw(3, 1, 0, 0);
}
}; // object