//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan graphics pipeline object.
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
  // Returns Vulkan object handle
  const VkPipeline &Handle() const;

private:
  VkPipeline Create(const VkGraphicsPipelineCreateInfo &create_info) const;
  // Reference to resource this object is created with
  const VkDevice &device_;
  const VkPipeline pipeline_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif