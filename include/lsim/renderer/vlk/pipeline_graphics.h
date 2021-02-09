//
// Created by Ivars Rusbergs in 2021
//
#ifndef LSIM_RENDERER_VLK_PIPELINE_GRAPHICS_H_
#define LSIM_RENDERER_VLK_PIPELINE_GRAPHICS_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class PipelineGraphics {
public:
  PipelineGraphics(const VkDevice &device,
                   const VkGraphicsPipelineCreateInfo &create_info);
  ~PipelineGraphics();
  PipelineGraphics(PipelineGraphics const &) = delete;
  PipelineGraphics operator=(PipelineGraphics const &) = delete;
  const VkPipeline &Get() const;

private:
  VkPipeline Create(const VkGraphicsPipelineCreateInfo &create_info) const;
  // Reference to resource this object is created with
  const VkDevice &device_;
  const VkPipeline pipeline_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif