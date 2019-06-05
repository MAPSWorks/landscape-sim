#pragma once
#include <vector>
#include "i_object.h"
#include <renderer/types.h>
#include <renderer/renderer.h>
#include <renderer/vlk/vertex_buffer.h>

// Data comes from vertex buffer
namespace object {
class TriangleVB : public IObject {
public:
    TriangleVB(renderer::Renderer& renderer);
    ~TriangleVB();
    void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const override;
private:
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription() const;
    std::vector<renderer::VertexPos2dColor> GetVertices() const;
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline catche
    const renderer::PipelineId pipeline_id_;
    const std::vector<renderer::VertexPos2dColor> vertices_;
    const renderer::vlk::VertexBuffer vertex_buffer_;
};
}; // object