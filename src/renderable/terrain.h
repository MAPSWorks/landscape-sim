#pragma once
#include <vector>
#include "i_renderable.h"
#include <base/matrix.h>
#include <renderer/types.h>
#include <renderer/renderer.h>
#include <renderer/shader_resources.h>
#include <renderer/vlk/vertex_buffer.h>
#include <renderer/vlk/index_buffer.h>
#include <renderer/vlk/uniform_buffer.h>
#include <renderer/vlk/descriptor_set_layout.h>

// Terrain representation as a renderable object
namespace renderable {
struct UniformBufferObject {
    t::Mat4 world_from_local;
};

class Terrain : public IRenderable {
public:
    Terrain(renderer::Renderer& renderer);
    virtual void InitDescriptorSets() override;
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameId frame_id) const override;
    virtual void UpdateUniformBuffer(renderer::FrameId frame_id) const override;
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
};
}; // renderable