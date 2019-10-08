#include "pipeline_layout.h"
#include <base/log.h>
#include <base/types.h>

namespace renderer::vlk {
PipelineLayout::PipelineLayout(const VkDevice& device, const Params& params) :
    device_(device),
    pipeline_layout_(Create(params)) {
    base::Log::Info("Renderer: pipeline layout for (" , params.layouts.size(), ") layouts created");
}

PipelineLayout::~PipelineLayout() {
    base::Log::Info("Renderer: pipeline layout destroying...");
    vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
}

const VkPipelineLayout& PipelineLayout::Get() const {
    return pipeline_layout_;
}

VkPipelineLayout PipelineLayout::Create(const Params& params) const {
    VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // Descriptor set layout
    pipeline_layout_create_info.setLayoutCount = static_cast<t::U32>(params.layouts.size());
    pipeline_layout_create_info.pSetLayouts = params.layouts.data();
    // Push constants
    pipeline_layout_create_info.pushConstantRangeCount = 0; // Optional
    pipeline_layout_create_info.pPushConstantRanges = nullptr; // Optional
    VkPipelineLayout pipeline_layout;
    ErrorCheck(vkCreatePipelineLayout(device_, &pipeline_layout_create_info, nullptr, &pipeline_layout));
    return pipeline_layout;
}
};  // renderer vlk