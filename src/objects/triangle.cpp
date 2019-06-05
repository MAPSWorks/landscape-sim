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

// Add command to given command buffer that is already in recording state
void Triangle::AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const {
    command_buffer.BindGraphicsPipeline(renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_));
    command_buffer.Draw(3, 1, 0, 0);
}

renderer::vlk::GraphicsPipeline::CreateParams Triangle::GetPipelineDescription() {
    auto description = renderer::vlk::GraphicsPipeline::CreateParams{
        // Pipeline descriptive name, should differ for different pipelines
        "Triangle pipeline",
        // Programmable stage
        {
            // Shader stages
            {"shaders/vert.spv", renderer::vlk::GraphicsPipeline::ShaderStage::kVertex },
            {"shaders/frag.spv", renderer::vlk::GraphicsPipeline::ShaderStage::kFragment },
        },
        // Vertex input params
        {
            // Vertex data is hardcoded into vertex shader
            {},{}
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