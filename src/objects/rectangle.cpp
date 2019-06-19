#include "rectangle.h"
#include <base/log.h>

namespace object {
Rectangle::Rectangle(renderer::Renderer& renderer) :
    renderer_(renderer),
    pipeline_id_(renderer_.GetPipelineManager().AddGraphicsPipeline(GetPipelineDescription(),
        renderer_.GetWindow().GetRenderPass(), renderer_.GetWindow().GetSwapchainObject().GetExtent())),
    vertices_(GetVertices()),
    indices_(GetIndices()),
    vertex_buffer_("rectangle vertices", renderer_.GetMemoryAllocator(),
        (renderer::vlk::BufferSize)(vertices_.size() * sizeof(vertices_[0])), vertices_.data()),
    index_buffer_("rectangle indices", renderer_.GetMemoryAllocator(),
        (renderer::vlk::BufferSize)(indices_.size() * sizeof(indices_[0])), indices_.data()) {
    base::Log::Info("Objects: rectangle created");
}

Rectangle::~Rectangle() {
    base::Log::Info("Objects: rectangle destroying...");
}

// Add command to given command buffer that is already in recording state
void Rectangle::AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const {
    command_buffer.BindGraphicsPipeline(renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_));
    command_buffer.BindVertexBuffer(vertex_buffer_.Get());
    command_buffer.BindIndexBuffer32(index_buffer_.Get());
    command_buffer.DrawIndexed(static_cast<uint32_t>(indices_.size()), 1, 0, 0, 0);
}

renderer::vlk::GraphicsPipeline::CreateParams Rectangle::GetPipelineDescription() const {
    auto description = renderer::vlk::GraphicsPipeline::CreateParams{
        // Pipeline descriptive name, should differ for different pipelines
        "Rectangle pipeline",
        // Programmable stage
        {
        // Shader stages
        {"shaders/vert_vb.spv", renderer::vlk::GraphicsPipeline::ShaderStage::kVertex },
        {"shaders/frag_vb.spv", renderer::vlk::GraphicsPipeline::ShaderStage::kFragment },
    },
    // Vertex input params
    {
        renderer::VertexPos2dColor::GetBindingDescription(),
        renderer::VertexPos2dColor::GetAttributeDescriptions(),
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

std::vector<renderer::VertexPos2dColor> Rectangle::GetVertices() const {
    std::vector<renderer::VertexPos2dColor> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };
    return vertices;
}

std::vector<uint32_t> Rectangle::GetIndices() const {
    const std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0
    };
    return indices;
}
}; // object