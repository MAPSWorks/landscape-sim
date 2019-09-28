#pragma once
#include <unordered_map>
#include <vulkan/vulkan.h>
#include "vlk/graphics_pipeline.h"

// Stores and manages rendering pipelines.
// Pipeline container is implemented using hash map, where
// pipeline name is used as a key into the map. This allows us to reuse 
// the piepeline in case we want to. Pipeline names are entered by hand
// and should differ for different pipelines.
namespace renderer {
using PipelineId = std::string;
class PipelineManager {
public:
    PipelineManager(const VkDevice& device);
    // Add pipeline to collection and return it's id.
    PipelineId AddGraphicsPipeline(const vlk::GraphicsPipeline::CreateParams& create_params, 
        const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent);
    const VkPipeline& GetGraphicsPipeline(const PipelineId& id) const;
    // Delete and creates pipelines with new extent and render pass.
    // Happens when window is resized.
    void RecreatePipelines(const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent);
private:
    // Pipeline manager is created for certain device and certain render pass
    // there for we grab reference to device, but render pass is passed on creation,
    // because it can change during lifetime of an application.
    // TODO: render passes are recreated with swapchain resize
    const VkDevice& device_;
    // Pipelines are stored in hash map with pipeline name being key.
    std::unordered_map<PipelineId, vlk::GraphicsPipeline> graphics_pipelines_;
};
}; // renderer