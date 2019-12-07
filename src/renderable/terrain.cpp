#include "terrain.h"
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <base/log.h>
#include <base/image_file.h>
#include <scene/types.h>

namespace renderable {
Terrain::Terrain(renderer::Renderer& renderer, const scene::View& view) :
    description_({ "textures/ps_height_1k.png",
        100.0f,
        0.1f, // Each 16-bit pixel unit (0 to 65535) corresponds to 0.1 meter
        160.0f }),// Spacing in meters between pixels in heightmap
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
    height_map_("terrain_height_texture", description_.height_map, renderer_, 0, renderer::Texture2D::DataFormat::kUInt),
    sampler_(renderer_.GetContext().device.Get(), renderer::vlk::Sampler::UsageMode::kHeightmap)
{
    base::Log::Info("Renderable: terrain created");
}

// After descriptor pool is created we can add and update descriptor sets
void Terrain::InitDescriptorSets() {
    // Add descriptor sets (inside sets for all frame-in-flights are created)
    descriptor_set_id_ = renderer_.GetShaderResources().AddDescriptorSet(descriptor_set_layout_.Get());
    // Bind resources to descriptor set
    std::vector<renderer::ShaderResources::DescrSetUpdateInfo> resources_to_bind;
    const auto bindings = GetDescriptorSetBindings();
    // NOTE: order of bindings and resource descriptions here should match!!!!!
    for (const auto& binding : bindings) {
        renderer::ShaderResources::DescrSetUpdateInfo resource;
        resource.type = binding.type;
        resource.count = binding.count;
        // Uniform buffer
        if (resource.type == renderer::vlk::DescriptorType::kUniformBuffer) {
            resource.buffer_id = uniform_buffer_id_;
            resource.buffer_offset = 0;
            // Whole buffer
            resource.buffer_range = 0;
        } else
        // Image sampler
        if (resource.type == renderer::vlk::DescriptorType::kCombinedImageSampler) {
            resource.image_view = height_map_.GetImageView();
            resource.image_sampler = sampler_.Get();
        }
        else {
            throw std::runtime_error("Renderable: unhandled resource type");
        }
        resources_to_bind.push_back(resource);
    }
    renderer_.GetShaderResources().UpdateDescriptorSet(descriptor_set_id_, resources_to_bind);
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
    UniformBufferObject ubo {};
    ubo.world_from_local = t::kMat4Identoty;
    /*
    ubo.world_from_local = glm::scale(t::kMat4Identoty, t::Vec3(description_.scale, 3.0, 
        description_.scale));
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
            MeshVertexType::GetBindingDescription(),
            MeshVertexType::GetAttributeDescriptions(),
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

// Generate vertices from height grid
// Vertices are defined in x-y plane as if it is x-z plane,
// because since we don't need to store height data here we can send 2d data
// to GPU and vertex shader will interpret and assign third dimension.
// Coordinates of terrain posts should be integer because they are used as tex coordinates
// in vertex shader to fetch texture data. Posts should start at 0 and go to 
// heightmap texture size - 1
std::vector<Terrain::MeshVertexType> Terrain::GetVertices() const {
    std::vector<MeshVertexType> vertices;
    //t::F32 tile_size = description_.horizontal_spacing * description_.scale;
    t::U32 row_count = 1025;
    t::U32 col_count = 1025;
    for (t::U32 row = 0; row < row_count - 1; ++row) {
        for (t::U32 col = 0; col < col_count - 1; ++col) {
            MeshVertexType vertice;
            vertice.position = { col , row };
            vertices.push_back(vertice);
        }
    }
    return vertices;
}

std::vector<t::U32> Terrain::GetIndices() const {
    std::vector<t::U32> indices;
    t::U32 row_count = 1025 - 1;
    t::U32 col_count = 1025 - 1;
    t::U32 indice = 0;
    for (t::U32 row = 0; row < row_count - 1; ++row) {
        for (t::U32 col = 0; col < col_count - 1; ++col) {
            // triangle 1
            indices.emplace_back(indice);
            indices.emplace_back(indice + 1); 
            indices.emplace_back(indice + col_count);
            // triangle 2
            indices.emplace_back(indice + 1);
            indices.emplace_back(indice + col_count);
            indices.emplace_back(indice + 1 + col_count);
            ++indice;
        }
        ++indice;
    }

    return indices;
}

// escribe bindings withing a descriptor set layout
std::vector<renderer::vlk::DescriptorSetLayout::Binding> Terrain::GetDescriptorSetBindings() const {
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> bindings;
    // Uniform buffer
    {
        renderer::vlk::DescriptorSetLayout::Binding binding;
        // Binding index (location), corresponds to layout(binding = n)  in shader
        binding.index = scene::DescriptorBidingId::kUniformBuffer;
        binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
        binding.stage = renderer::vlk::ShaderStage::kVertex;
        binding.count = 1;
        bindings.push_back(binding);
    }
    // Combined image sampler
    {
        renderer::vlk::DescriptorSetLayout::Binding binding;
        // Binding index (location), corresponds to layout(binding = n)  in shader
        binding.index = scene::DescriptorBidingId::kCombinedImageSampler;
        binding.type = renderer::vlk::DescriptorType::kCombinedImageSampler;
        binding.stage = renderer::vlk::ShaderStage::kVertex;
        binding.count = 1;
        bindings.push_back(binding);
    }

    return bindings;
}
}; // renderable