#pragma once
#include "i_object.h"
#include <renderer/renderer.h>

namespace object {
class Triangle : public IObject {
public:
    Triangle(renderer::Renderer& renderer);
    ~Triangle();
    void RecordToCommandBuffer() const override;
private:
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    // Only this object knows what pipeline it needs
    const renderer::vlk::GraphicsPipeline::CreateParams pipeline_description_;
    // Id of pipeline that is going to be used for rendering
    const renderer::PipelineId pipeline_id_;
};
}; // object