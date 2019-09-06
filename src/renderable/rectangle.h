#pragma once
#include <vector>
#include "i_renderable.h"
#include <renderer/types.h>
#include <renderer/renderer.h>
#include <renderer/vlk/vertex_buffer.h>
#include <renderer/vlk/index_buffer.h>

// Vertex and index buffers
namespace renderable {
class Rectangle : public IRenderable {
public:
    Rectangle(renderer::Renderer& renderer);
    ~Rectangle();
    void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const override;
private:
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription() const;
    std::vector<renderer::VertexPos2dColor> GetVertices() const;
    std::vector<uint32_t> GetIndices() const;
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline catche
    const renderer::PipelineId pipeline_id_;
    const std::vector<renderer::VertexPos2dColor> vertices_;
    const std::vector<uint32_t> indices_;
    const renderer::vlk::VertexBuffer vertex_buffer_;
    const renderer::vlk::IndexBuffer index_buffer_;
};
}; // renderable