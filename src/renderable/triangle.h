#pragma once
#include "i_renderable.h"
#include <renderer/renderer.h>

// Data hard coded in vertex shader
namespace renderable {
class Triangle : public IRenderable {
public:
    Triangle(renderer::Renderer& renderer);
    ~Triangle();
    void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const override;
private:
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription();
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline catche
    const renderer::PipelineId pipeline_id_;
};
}; // renderable