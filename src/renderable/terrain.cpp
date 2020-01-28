#include "terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <base/log.h>
#include <base/image_file.h>
#include <scene/types.h>

namespace renderable {
Terrain::Terrain(renderer::Renderer& renderer, const scene::View& view) :
    /*
    description_({ "textures/ps_height_1k.png",
        0.1f,
        160.0f }),*/
    description_({ "textures/height_flat_64.png",
    0.02f,
    160.0f }), 
    renderer_(renderer),
    height_map_("terrain_height_texture", description_.height_map, renderer_, 0, renderer::Texture2D::DataFormat::kUInt),
    vertices_(GetVertices()),
    indices_(GetIndices()),
    vertex_buffer_("terrain vertices", renderer_, (renderer::vlk::BufferSize)(vertices_.size() * sizeof(vertices_[0])), vertices_.data()),
    index_buffer_("terrain indices", renderer_, (renderer::vlk::BufferSize)(indices_.size() * sizeof(indices_[0])), indices_.data()),
    descriptor_set_layout_view_(view.GetDescriptorSetLayout()),
    descriptor_set_layout_(renderer_.GetShaderResources().GetDescriptorSetLayoutCache().AddDescriptorSetLayout(GetDescriptorSetBindings())),
    pipeline_id_(renderer_.GetPipelineManager().AddGraphicsPipeline(GetPipelineDescription(),
        renderer_.GetWindow().GetRenderPass(), renderer_.GetWindow().GetSwapchainObject().GetExtent())),
    uniform_buffer_id_(renderer_.GetShaderResources().AddUniformBuffer("uniform buffer", sizeof(UniformData))),
    sampler_dummy_(renderer_.GetContext().device.Get(), renderer::vlk::Sampler::UsageMode::kHeightmap),
    base_texture_("terrain_base_texture", "textures/texture.jpg", renderer_),
    base_sampler_(renderer_.GetContext().device.Get())
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
    {
        renderer::ShaderResources::DescrSetUpdateInfo resource;
        const auto& binding = bindings.at(0);
        resource.type = binding.type;
        resource.count = binding.count;
        resource.buffer_id = uniform_buffer_id_;
        resource.buffer_offset = 0;
        // Whole buffer
        resource.buffer_range = 0;
        resources_to_bind.push_back(resource);
    }
    {
        renderer::ShaderResources::DescrSetUpdateInfo resource;
        const auto& binding = bindings.at(1);
        resource.type = binding.type;
        resource.count = binding.count;
        resource.image_view = height_map_.GetImageView();
        resource.image_sampler = sampler_dummy_.Get();
        resources_to_bind.push_back(resource);
    }
    {
        renderer::ShaderResources::DescrSetUpdateInfo resource;
        const auto& binding = bindings.at(2);
        resource.type = binding.type;
        resource.count = binding.count;
        resource.image_view = base_texture_.GetImageView();
        resource.image_sampler = base_sampler_.Get();
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
    UniformData ubo {};
    //ubo.world_from_local = t::kMat4Identoty;
    ubo.world_from_local = glm::scale(t::kMat4Identoty, t::Vec3(description_.horizontal_spacing, 
        description_.height_unit_size,
        description_.horizontal_spacing) / scene::kMetersPerUnit);
    renderer_.GetShaderResources().GetkUniformBuffer(uniform_buffer_id_, frame_id).Update(&ubo);
}

void Terrain::UpdateGUI() {
    //static float f = 0.0f;
    //static int counter = 0;

    ImGui::Begin("Terrain");
    /*
    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)


    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    float float_val = 0;
    ImGui::InputFloat("input float ", &float_val);
    ImGui::Text("float = %f", float_val);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    */
    ImGui::End();
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

// Generate vertices from height grid
// Vertices are defined in x-y plane as if it is x-z plane,
// because since we don't need to store height data here we can send 2d data
// to GPU and vertex shader will interpret and assign third dimension.
// Coordinates of terrain posts should be integer because they are used as tex coordinates
// in vertex shader to fetch texture data. Posts should start at 0 and go to 
// heightmap texture size - 1
std::vector<Terrain::MeshVertexType> Terrain::GetVertices() const {
    std::vector<MeshVertexType> vertices;
    t::U32 row_count = height_map_.GetImage().GetExtent().height;
    t::U32 col_count = height_map_.GetImage().GetExtent().width;;
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
    t::U32 row_count = height_map_.GetImage().GetExtent().height - 1;
    t::U32 col_count = height_map_.GetImage().GetExtent().width - 1;
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

// Describe bindings within a descriptor set layout
std::vector<renderer::vlk::DescriptorSetLayout::Binding> Terrain::GetDescriptorSetBindings() const {
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> bindings;
    // Uniform buffer
    {
        renderer::vlk::DescriptorSetLayout::Binding binding;
        // Binding index (location), corresponds to layout(binding = n)  in shader
        binding.index = 0;
        binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
        binding.stage = renderer::vlk::ShaderStage::kVertex;
        binding.count = 1;
        bindings.push_back(binding);
    }
    // Combined image sampler (height map)
    {
        renderer::vlk::DescriptorSetLayout::Binding binding;
        // Binding index (location), corresponds to layout(binding = n)  in shader
        binding.index = 1;
        binding.type = renderer::vlk::DescriptorType::kCombinedImageSampler;
        binding.stage = renderer::vlk::ShaderStage::kVertex;
        binding.count = 1;
        bindings.push_back(binding);
    }
    // Combined image sampler (base texture)
    {
        renderer::vlk::DescriptorSetLayout::Binding binding;
        // Binding index (location), corresponds to layout(binding = n)  in shader
        binding.index = 2;
        binding.type = renderer::vlk::DescriptorType::kCombinedImageSampler;
        binding.stage = renderer::vlk::ShaderStage::kFragment;
        binding.count = 1;
        bindings.push_back(binding);
    }

    return bindings;
}
}; // renderable