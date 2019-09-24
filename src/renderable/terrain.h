#pragma once
#include "i_renderable.h"
#include <renderer/renderer.h>
#include <base/matrix.h>

// Terrain representation as a renderable object
namespace renderable {
class Terrain : public IRenderable {
public:
    Terrain(renderer::Renderer& renderer);
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const override;
private:
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription();
    // Height grid representation as 2d matrix
    base::Matrix<t::F32> height_grid_;
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline catche
    const renderer::PipelineId pipeline_id_;
};
}; // renderable