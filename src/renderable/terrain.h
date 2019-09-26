#pragma once
#include <vector>
#include "i_renderable.h"
#include <renderer/types.h>
#include <renderer/renderer.h>
#include <renderer/vlk/vertex_buffer.h>
#include <renderer/vlk/index_buffer.h>
#include <renderer/vlk/descriptor_set_layout.h>
#include <base/matrix.h>

struct UniformBufferObject {
    t::Mat4 world_from_local;
    t::Mat4 view_from_world;
    t::Mat4 projection_from_view;
};

// Terrain representation as a renderable object
namespace renderable {
class Terrain : public IRenderable {
public:
    Terrain(renderer::Renderer& renderer);
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const override;
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
    // Describes uniform buffer object to pipeline.
    // This object should be created before pipeline.
    // This object should be alive until pipeline is destroyed, 
    // because handles to DescriptorSetLayout are used in the pipeline.
    const renderer::vlk::DescriptorSetLayout descriptor_set_layout_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline catche
    const renderer::PipelineId pipeline_id_;

};
}; // renderable