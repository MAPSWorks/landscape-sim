#include "terrain.h"
#include <base/log.h>

namespace renderable {
Terrain::Terrain(renderer::Renderer& renderer) :
    height_grid_(GenerateHeightGrid(5)),
    renderer_(renderer),
    pipeline_id_(renderer_.GetPipelineManager().AddGraphicsPipeline(GetPipelineDescription(),
        renderer_.GetWindow().GetRenderPass(), renderer_.GetWindow().GetSwapchainObject().GetExtent())),
    vertices_(GetVertices()),
    vertex_buffer_("terrain vertices", renderer_.GetMemoryAllocator(),
        (renderer::vlk::BufferSize)(vertices_.size() * sizeof(vertices_[0])), vertices_.data()) {
    base::Log::Info("Renderable: terrain created");
}

// Add command to given command buffer that is already in recording state
void Terrain::AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const {
    command_buffer.BindGraphicsPipeline(renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_));
    command_buffer.BindVertexBuffer(vertex_buffer_.Get());
    command_buffer.Draw(static_cast<uint32_t>(vertices_.size()), 1, 0, 0);
}

renderer::vlk::GraphicsPipeline::CreateParams Terrain::GetPipelineDescription() {
    auto description = renderer::vlk::GraphicsPipeline::CreateParams{
        // Pipeline descriptive name, should differ for different pipelines
        "Terrain pipeline",
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
            {}
        }
    };

    return description;
}

// Generate height grid values for given area size and return
base::Matrix<t::F32> Terrain::GenerateHeightGrid(t::U16 size) const {
    base::Matrix<t::F32> height_grid(size, size);
    height_grid(0, 0) = 1.0;
    height_grid(size - 1, size - 1) = 2.0;
    return height_grid;
}

// Generate vertices from height grid
std::vector<renderer::VertexPos2dColor> Terrain::GetVertices() const {
    std::vector<renderer::VertexPos2dColor> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.5f, 0.0f}},
        {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
        };
    return vertices;
}
}; // renderable