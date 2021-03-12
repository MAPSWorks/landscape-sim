//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/pipeline_layout.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
PipelineLayout::PipelineLayout(VkDevice device,
                               const VkPipelineLayoutCreateInfo &create_info)
    : context_device_(device), pipeline_layout_(Create(create_info)) {
  base::Log::Info("renderer", "pipeline layout", "created");
}

PipelineLayout::~PipelineLayout() {
  base::Log::Info("renderer", "pipeline layout", "destroying..");
  vkDestroyPipelineLayout(context_device_, pipeline_layout_, nullptr);
}

VkPipelineLayout PipelineLayout::Handle() const { return pipeline_layout_; }

VkPipelineLayout
PipelineLayout::Create(const VkPipelineLayoutCreateInfo &create_info) const {
  VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
  ErrorCheck(vkCreatePipelineLayout(context_device_, &create_info, nullptr,
                                    &pipeline_layout));
  return pipeline_layout;
}

} // namespace lsim::renderer::vlk