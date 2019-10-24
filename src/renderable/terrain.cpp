#include "terrain.h"
#include <chrono>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include <base/log.h>
#include <scene/types.h>

namespace renderable {
Terrain::Terrain(renderer::Renderer& renderer, const scene::View& view) :
    height_grid_(GenerateHeightGrid(20)),
    renderer_(renderer),
    vertices_(GetVertices()),
    indices_(GetIndices()),
    vertex_buffer_("terrain vertices", renderer_, (renderer::vlk::BufferSize)(vertices_.size() * sizeof(vertices_[0])), vertices_.data()),
    index_buffer_("terrain indices", renderer_, (renderer::vlk::BufferSize)(indices_.size() * sizeof(indices_[0])), indices_.data()),
    descriptor_set_layout_view_(view.GetDescriptorSetLayout()),
    descriptor_set_layout_(renderer_.GetShaderResources().GetDescriptorSetLayoutCache().AddDescriptorSetLayout(GetDescriptorSetBindings())),
    pipeline_id_(renderer_.GetPipelineManager().AddGraphicsPipeline(GetPipelineDescription(),
        renderer_.GetWindow().GetRenderPass(), renderer_.GetWindow().GetSwapchainObject().GetExtent())),
    uniform_buffer_id_(renderer_.GetShaderResources().AddUniformBuffer("uniform buffer", sizeof(UniformBufferObject))),

    texture_("test_texture", "textures/texture.jpg", renderer_)
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
void Terrain::AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameManager::FrameId frame_id) const {
    command_buffer.BindGraphicsPipeline(renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_).Get());
    command_buffer.BindVertexBuffer(vertex_buffer_.Get());
    command_buffer.BindIndexBuffer32(index_buffer_.Get());
    command_buffer.BindGraphicsDescriptorSet(renderer_.GetShaderResources().GetDescriptorSet(descriptor_set_id_, frame_id).Get(),
        renderer_.GetPipelineManager().GetGraphicsPipeline(pipeline_id_).GetLayout().Get(), scene::DescruptorSetSlotId::kPerObject);
    command_buffer.DrawIndexed(static_cast<t::U32>(indices_.size()), 1, 0, 0, 0);
}

void Terrain::UpdateUniformBuffer(renderer::FrameManager::FrameId frame_id) const {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    UniformBufferObject ubo  {};
    ubo.world_from_local = glm::rotate(t::kMat4Identoty, time * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    /*
    UniformBufferObject ubo{};
    ubo.world_from_local = t::kMat4Identoty;
    */
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
            renderer::VertexPos3dColor::GetBindingDescription(),
            renderer::VertexPos3dColor::GetAttributeDescriptions(),
        },
        // Fixed function state
        // Primitive assembly
        {
            // Primitive topology
            renderer::vlk::GraphicsPipeline::PrimitiveTopology::kTriangleList
        },
        // Rasterization
        {
            renderer::vlk::GraphicsPipeline::PolygonMode::kLine,
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
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<t::F32> dist(1.0f, 5.0f);

    base::Matrix<t::F32> height_grid(size, size);
    for (t::U32 row = 0; row < height_grid.GetRows(); ++row) {
        for (t::U32 col = 0; col < height_grid.GetCols(); ++col) {
            height_grid(row, col) = dist(mt) + col / height_grid.GetCols();
        }
    }
    return height_grid; 
}

// Generate vertices from height grid
std::vector<renderer::VertexPos3dColor> Terrain::GetVertices() const {
    std::vector<renderer::VertexPos3dColor> vertices;
    t::F32 tile_size = 4.0f;
    t::F32 terrain_half_width = (height_grid_.GetCols() * tile_size) / 2.0f;
    t::F32 terrain_half_height = (height_grid_.GetRows() * tile_size) / 2.0f;
    for (t::U32 row = 0; row < height_grid_.GetRows(); ++row) {
        for (t::U32 col = 0; col < height_grid_.GetCols(); ++col) {
            renderer::VertexPos3dColor vertice;
            vertice.position = { row * tile_size - terrain_half_width, height_grid_(row, col), col * tile_size - terrain_half_height };
            vertice.color = { row / (t::F32)height_grid_.GetRows(), col / (t::F32)height_grid_.GetCols(),  0.2f };
            vertices.push_back(vertice);
        }
    }
    return vertices;
}

std::vector<t::U32> Terrain::GetIndices() const {
    std::vector<t::U32> indices;
    t::U32 indice = 0;
    for (t::U32 row = 0; row < height_grid_.GetRows() - 1; ++row) {
        for (t::U32 col = 0; col < height_grid_.GetCols() - 1; ++col) {
            // triangle 1
            indices.emplace_back(indice);
            indices.emplace_back(indice + 1); 
            indices.emplace_back(indice + (t::U32)height_grid_.GetCols());
            // triangle 2
            indices.emplace_back(indice + 1);
            indices.emplace_back(indice + (t::U32)height_grid_.GetCols());
            indices.emplace_back(indice + 1 + (t::U32)height_grid_.GetCols());
            ++indice;
        }
        ++indice;
    }

    return indices;
}

// escribe bindings withing a descriptor set layout
std::vector<renderer::vlk::DescriptorSetLayout::Binding> Terrain::GetDescriptorSetBindings() const {
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> bindings;
    renderer::vlk::DescriptorSetLayout::Binding binding;
    // Binding index (location), corresponds to layout(binding = n)  in shader
    binding.index = scene::ViewDescriptorBidingId::kUniformBuffer;
    binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
    binding.stage = renderer::vlk::ShaderStage::kVertex;
    binding.count = 1;
    bindings.push_back(binding);
    return bindings;
}
}; // renderable