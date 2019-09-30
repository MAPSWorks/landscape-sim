#pragma once
#include <vector>
#include "i_renderable.h"
#include <renderer/types.h>
#include <renderer/renderer.h>
#include <renderer/vlk/vertex_buffer.h>
#include <renderer/vlk/index_buffer.h>
#include <renderer/vlk/uniform_buffer.h>
#include <renderer/vlk/descriptor_set_layout.h>
#include <base/matrix.h>
#include <renderer/vlk/descriptor_sets.h>

// Terrain representation as a renderable object
namespace renderable {
class Terrain : public IRenderable {
public:
    Terrain(renderer::Renderer& renderer);
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const override;
    virtual void UpdateUniformBuffer(const renderer::vlk::UniformBuffer& cuniform_buffer) const override;
private:
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription();
    // Generate and return height grid populated with height values that define
    // terrain height.
    base::Matrix<t::F32> GenerateHeightGrid(t::U16 size) const;
    std::vector<renderer::VertexPos2dColor> GetVertices() const;
    std::vector<t::U32> GetIndices() const;
    // Describe how are descriptor set layout bound to pipeline
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> GetDescriptorSetBindings() const;
    // Height grid representation as 2d matrix
    const base::Matrix<t::F32> height_grid_;
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    const std::vector<renderer::VertexPos2dColor> vertices_;
    const std::vector<t::U32> indices_;
    const renderer::vlk::VertexBuffer vertex_buffer_;
    const renderer::vlk::IndexBuffer index_buffer_;
    // A reference to an object that describes resource binding to pipeline
    // Actual objects are stored in descriptor set layout cache for reuse
    const renderer::vlk::DescriptorSetLayout& descriptor_set_layout_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline cache
    const renderer::PipelineId pipeline_id_;
    renderer::vlk::DescriptorSets descriptor_sets_;
};
}; // renderable