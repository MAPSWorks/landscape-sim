#pragma once
#include <vector>
#include <memory>
#include <vulkan/vulkan.h>
#include "vlk/graphics_pipeline.h"

namespace renderer {
using PipelineId = uint32_t;
// Stores and manages pipelines required to render a particular scene
class PipelineManager {
public:
    PipelineManager(const VkDevice& device);
    // Add pipeline to collection and return it's id
    PipelineId AddGraphicsPipeline(const vlk::GraphicsPipeline::CreateParams& create_params, 
        const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent);
    const VkPipeline& GetGraphicsPipeline(PipelineId id) const;
    // Delete and creates pipelines with new extent and render pass
    // Happens when window is resized
    void RecreatePipelines(const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent);
private:
    // Pipeline manager is created for certain device and certain render pass
    // there for we grab reference to device, but render pass is passed on creation,
    // because it can change during lifetime of an application
    // TODO: render passes are recreated with swapchain resize
    const VkDevice& device_;
    std::vector<std::unique_ptr<vlk::GraphicsPipeline>> graphics_pipelines_;
};
}; // renderer