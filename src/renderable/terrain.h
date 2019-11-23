#pragma once
#include <vector>
#include <string_view>
#include <base/matrix.h>
#include <renderer/types.h>
#include <renderer/renderer.h>
#include <renderer/shader_resources.h>
#include <renderer/vertex_buffer.h>
#include <renderer/index_buffer.h>
#include <renderer/vlk/uniform_buffer.h>
#include <renderer/vlk/descriptor_set_layout.h>
#include <scene/view.h>
#include <renderer/texture2d.h>
#include <renderer/vlk/sampler.h>
#include "i_renderable.h"


// Terrain representation as a renderable object
namespace renderable {
struct UniformBufferObject {
    t::Mat4 world_from_local;
};

class Terrain : public IRenderable {
public:
    // Data that describes the terrain
    struct Description {
        // Texture that describes terrain height values at selected points
        // Usually a 16-bit square grayscale texture
        std::string_view height_map;
        // Scale of the whole terrain compared to original
        t::F32 scale;
        // How much world units the single pixel intensity value represents
        t::F32 height_unit_size;
        // Spacing in world units between each pixel value
        t::F32 horizontal_spacing;
    };
    Terrain(renderer::Renderer& renderer, const scene::View& view);
    virtual void InitDescriptorSets() override;
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameManager::FrameId frame_id) const override;
    virtual void UpdateUniformBuffer(renderer::FrameManager::FrameId frame_id) const override;
private:
    using HeightGrid = base::Matrix<t::F32>;
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription();
    // Generate and return height grid populated with height values that define
    // terrain height.
    HeightGrid GenerateHeightGrid() const;
    std::vector<renderer::VertexPos3dColorTex> GetVertices() const;
    std::vector<t::U32> GetIndices() const;
    // Describe how are descriptor set layout bound to pipeline
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> GetDescriptorSetBindings() const;
    // Description of terrain
    const Description description_;
    // Height grid representation as 2d matrix
    const HeightGrid height_grid_;
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    const std::vector<renderer::VertexPos3dColorTex> vertices_;
    const std::vector<t::U32> indices_;
    const renderer::VertexBuffer vertex_buffer_;
    const renderer::IndexBuffer index_buffer_;
    // A reference to an object that describes resource binding to pipeline
    // Actual objects are stored in descriptor set layout cache for reuse
    const renderer::vlk::DescriptorSetLayout& descriptor_set_layout_view_;
    const renderer::vlk::DescriptorSetLayout& descriptor_set_layout_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline cache
    const renderer::PipelineId pipeline_id_;
    // Index that represents uniform buffer created from shader resource module
    // Id because there are as many as frames-in-flight
    const renderer::ShaderResources::UniformBufferId uniform_buffer_id_;
    // Index that represents descriptor set created from shader resource module
    // Id because there are as many as frames-in-flight
    // This is not const because we can assign this value only after constructor initializer list.
    renderer::ShaderResources::DescrSetId descriptor_set_id_;
    // Test
    const renderer::Texture2D texture_;
    const renderer::vlk::Sampler sampler_;
};
}; // renderable