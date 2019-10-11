#include "terrain.h"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <base/log.h>

namespace renderable {
Terrain::Terrain(renderer::Renderer& renderer) :
    height_grid_(GenerateHeightGrid(5)),
    renderer_(renderer),
    vertices_(GetVertices()),
    indices_(GetIndices()),
    vertex_buffer_("terrain vertices", renderer_.GetMemoryAllocator(),
        (renderer::vlk::BufferSize)(vertices_.size() * sizeof(vertices_[0])), vertices_.data()),
    index_buffer_("terrain indices", renderer_.GetMemoryAllocator(),
        (renderer::vlk::BufferSize)(indices_.size() * sizeof(indices_[0])), indices_.data()),
    descriptor_set_layout_view_(renderer_.GetShaderResources().GetDescriptorSetLayoutCache().AddDescriptorSetLayout(GetDescriptorSetBindings())),
    descriptor_set_layout_(renderer_.GetShaderResources().GetDescriptorSetLayoutCache().AddDescriptorSetLayout(GetDescriptorSetBindings())),
    pipeline_id_(renderer_.GetPipelineManager().AddGraphicsPipeline(GetPipelineDescription(),
        renderer_.GetWindow().GetRenderPass(), renderer_.GetWindow().GetSwapchainObject().GetExtent())),
    uniform_buffer_id_(renderer_.GetShaderResources().AddUniformBuffer("uniform buffer", sizeof(UniformBufferObject)))
{
    base::Log::Info("Renderable: terrain created");
}

// After descriptor pool is created we can add and update descriptor sets
void Terrain::InitDescriptorSets() {
    // Add descriptor sets (inside sets for all frame-in-flights are created)
    descriptor_set_id_ = renderer_.GetShaderResources().AddDescriptorSet(descriptor_set_layout_.Get());
    // Bind uniform buffer to descriptor set
    renderer_.GetShaderResources().UpdateDescriptorSetWithUniformBuffer(descriptor_set_id_, uniform_buffer_id_);
}

// Add command to given command buffer that is already in recording state
void Terrain::AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameId frame_id) const {
    command_buffer.BindGraphicsPipeline(renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_).Get());
    command_buffer.BindVertexBuffer(vertex_buffer_.Get());
    command_buffer.BindIndexBuffer32(index_buffer_.Get());
    command_buffer.BindGraphicsDescriptorSet(renderer_.GetShaderResources().GetDescriptorSet(descriptor_set_id_, frame_id).Get(),
        renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_).GetLayout().Get(), 1);
    command_buffer.DrawIndexed(static_cast<t::U32>(indices_.size()), 1, 0, 0, 0);
}

void Terrain::UpdateUniformBuffer(renderer::FrameId frame_id) const {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    UniformBufferObject ubo  {};
    ubo.world_from_local = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    renderer_.GetShaderResources().GetkUniformBuffer(uniform_buffer_id_, frame_id).Update(&ubo);
}

renderer::vlk::GraphicsPipeline::CreateParams Terrain::GetPipelineDescription() {
    auto description = renderer::vlk::GraphicsPipeline::CreateParams{
        // Pipeline id name, should differ for different pipelines.
        // In case multiple instances of this class is used, only one actual
        // pipeline will be used by the pipeline manager.
        "terrain",
        // Programmable stage
        {
            // Shader stages
            {"shaders/terrain_vert.spv", renderer::vlk::ShaderStage::kVertex },
            {"shaders/terrain_frag.spv", renderer::vlk::ShaderStage::kFragment },
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
            // Vector of descriptor set layouts that are used to create pipeline layout
            { descriptor_set_layout_view_.Get(), 
              descriptor_set_layout_.Get() }
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
    const std::vector<renderer::VertexPos2dColor> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };
    return vertices;
}

std::vector<t::U32> Terrain::GetIndices() const {
    const std::vector<t::U32> indices = {
        0, 1, 2, 2, 3, 0
    };
    return indices;
}

// escribe bindings withing a descriptor set layout
std::vector<renderer::vlk::DescriptorSetLayout::Binding> Terrain::GetDescriptorSetBindings() const {
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> bindings;
    renderer::vlk::DescriptorSetLayout::Binding binding;
    // Binding index (location), corresponds to layout(binding = n)  in shader
    binding.index = 0;
    binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
    binding.stage = renderer::vlk::ShaderStage::kVertex;
    binding.count = 1;
    bindings.push_back(binding);
    return bindings;
}
}; // renderable