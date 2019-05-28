#include "triangle.h"
#include <base/log.h>

namespace object {
Triangle::Triangle(renderer::Renderer& renderer) :
    renderer_(renderer),
    pipeline_id_(renderer_.GetPipelineManager().AddGraphicsPipeline(GetPipelineDescription(), 
        renderer_.GetWindow().GetRenderPass(), renderer_.GetWindow().GetSwapchainObject().GetExtent()))  {
    base::Log::Info("Objects: triangle created");
}

Triangle::~Triangle() {
    base::Log::Info("Objects: triangle destroying...");
}

// Command buffer and render pass is already started
void Triangle::RecordToCommandBuffer(uint32_t buffer_index) const {
    // For rendering triangle
    // Assuming we have nnly one set of command buffers
    renderer_.GetCommandBuffers().BindGraphicsPipeline(buffer_index, renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_));
    renderer_.GetCommandBuffers().Draw(buffer_index, 3, 1, 0, 0);
}

renderer::vlk::GraphicsPipeline::CreateParams Triangle::GetPipelineDescription()
{
    auto description = renderer::vlk::GraphicsPipeline::CreateParams{
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
    };

    return description;
}
}; // object