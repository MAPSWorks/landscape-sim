#pragma once
#include <renderer/context.h>
#include <renderer/vlk/graphics_pipeline.h>

namespace graphics::objects {
class Triangle {
public:
    Triangle(const renderer::Context& context);
    ~Triangle();
    void Draw() const;
private:
    const renderer::Context& context_;
    const renderer::vlk::GraphicsPipeline graphics_pipeline_;
};
}; //graphics