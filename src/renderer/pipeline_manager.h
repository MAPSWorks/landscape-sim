#pragma once
#include <vector>
#include <memory>
#include <vulkan/vulkan.h>
#include "vlk/graphics_pipeline.h"
#include"types.h"

namespace renderer {
// Stores and manages pipelines required to render a particular scene
class PipelineManager {
public:
    PipelineManager(const VkDevice& device, const VkRenderPass& render_pass);
    ~PipelineManager();
    // Add pipeline to collection and return it's id
    PipelineId AddGraphicsPipeline(const vlk::GraphicsPipeline::CreateParams& create_params);
    const VkPipeline& GetGraphicsPipeline(PipelineId id) const;
private:
    // Pipeline manager is created for certain device and certain render pass
    // there for we grab references on init
    const VkDevice& device_;
    const VkRenderPass& render_pass_;
    std::vector<std::unique_ptr<vlk::GraphicsPipeline>> graphics_pipelines_;
};
}; // renderer