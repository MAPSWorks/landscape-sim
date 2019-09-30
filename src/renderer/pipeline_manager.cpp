#include "pipeline_manager.h"
#include <utility>
#include <vector>
#include <base/log.h>

namespace renderer {
PipelineManager::PipelineManager(const VkDevice& device) :
    device_(device) {
    base::Log::Info("Renderer: pipeline manager initialized");
}

// Adds pipeline and returns id by which it is uniquelly identifieable.
// If pipeline name already exists return the same existing id and don't add anything new.
PipelineId PipelineManager::AddGraphicsPipeline(const vlk::GraphicsPipeline::CreateParams& create_params,
    const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent) {
    const PipelineId pipeline_id = create_params.name;
    // Check if given key already exists in the map.
    // It would be possible to skip this check, but unnecessary object construction will
    // happen in case there is no need to add that object.
    if (graphics_pipelines_.find(pipeline_id) == graphics_pipelines_.end()) {
        // Piecewise construct allows to create object in-place without the need to move them
        graphics_pipelines_.emplace(std::piecewise_construct,
            std::forward_as_tuple(pipeline_id),
            std::forward_as_tuple(device_, render_pass, swapchain_extent, create_params));
        base::Log::Info("Renderer: new pipeline '", pipeline_id, "' added by manager");
    }
    else {
        base::Log::Info("Renderer: no pipeline  was added by manager, existing '", pipeline_id, "' will be reused");
    }

    return pipeline_id;
}

// If no element is found, exception will be thrown.
const vlk::GraphicsPipeline& PipelineManager::GetGraphicsPipeline(const PipelineId& id) const {
    return graphics_pipelines_.at(id);
}

// Erase pipelines and recreate them with new parameter values.
void PipelineManager::RecreatePipelines(const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent) {
    // Gather the keys and parameters of existing pipelines
    std::vector<std::pair<PipelineId, vlk::GraphicsPipeline::CreateParams>> recreation_params;
    for (auto& pipeline : graphics_pipelines_) {
        recreation_params.push_back(std::make_pair(pipeline.first, pipeline.second.GetCreateParams()));
    }
    // Clear existsg pipelines
    graphics_pipelines_.clear();
    // Recreate them
    for (auto& recreation : recreation_params) {
        // Piecewise construct allows to create object in-place without the need to move them
        graphics_pipelines_.emplace(std::piecewise_construct,
            std::forward_as_tuple(recreation.first),
            std::forward_as_tuple(device_, render_pass, swapchain_extent, recreation.second));
    }
}
}; // renderer