#include "pipeline_manager.h"
#include <algorithm> 
#include <base/log.h>

namespace renderer {
PipelineManager::PipelineManager(const VkDevice& device) :
    device_(device) {
    base::Log::Info("Renderer: pipeline manager initialized");
}

// Adds pipeline and returns id by which it is uniquelly identifieable
// If pipeline already exists return the same existing id and don't add anything new
PipelineId PipelineManager::AddGraphicsPipeline(const vlk::GraphicsPipeline::CreateParams& create_params,
    const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent) {
    // Compare if pipeline exists by pipeline name
    // TODO: this might not be safe because pipeline names can match even when pipelines differ
    // if not entered correctly or forgoten
    const auto pipeline_iter = std::find_if(graphics_pipelines_.begin(), graphics_pipelines_.end(),
        [&create_params](const std::unique_ptr<vlk::GraphicsPipeline> & obj) 
        { return obj->GetName() == create_params.name; });
    // No such pipeline exists
    if (pipeline_iter == graphics_pipelines_.end()) {
        // Create pipelines on heap and store only unique ptr/.
        // Object ownership is delegated tounique ptr it was last assigned to
        graphics_pipelines_.push_back(std::make_unique<vlk::GraphicsPipeline>(device_, render_pass, swapchain_extent, create_params));
        // Pipeline id is element index just inserted 
        // TODO: this is wrong for many reasons, cannot reorder etc, should use unordered map instead
        const auto inserted_id = graphics_pipelines_.size() - 1;
        base::Log::Info("Renderer: graphics pipeline '", create_params.name ,"' id - '", inserted_id, "' added to pipeline manager");
        return static_cast<PipelineId>(inserted_id);
    }
    // Pipeline like this already exists
    else {
        const auto existing_index = std::distance(graphics_pipelines_.begin(), pipeline_iter);
        base::Log::Info("Renderer: graphics pipeline '", create_params.name, "' id - '", existing_index, "' already exists in pipeline manager");
        return static_cast<PipelineId>(existing_index);
    }
}

const VkPipeline& PipelineManager::GetGraphicsPipeline(PipelineId id) const {
    return graphics_pipelines_.at(id).get()->Get();
}

void PipelineManager::RecreatePipelines(const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent) {
    for (size_t i = 0; i < graphics_pipelines_.size(); i++) {
        auto& pipeline = graphics_pipelines_.at(i);
        // Get parameters this pipeline was created with
        const auto create_params = pipeline.get()->GetCreateParams();
        // Destroy eisting pipeline
        pipeline.reset();
        // Create the new one instead keeping the index the same
        pipeline = std::make_unique<vlk::GraphicsPipeline>(device_, render_pass, swapchain_extent, create_params);
    }
}
}; // renderer