#pragma once
#include "i_object.h"
#include <renderer/renderer.h>

namespace object {
class Triangle : public IObject {
public:
    Triangle(renderer::Renderer& renderer);
    ~Triangle();
    void RecordToCommandBuffer(uint32_t buffer_index) const override;
private:
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription();
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    // Id of pipeline that is going to be used for rendering
    const renderer::PipelineId pipeline_id_;
};
}; // object