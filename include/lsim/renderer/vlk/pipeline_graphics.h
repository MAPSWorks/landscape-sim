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
  PipelineGraphics(VkDevice device,
                   const VkGraphicsPipelineCreateInfo &create_info);
  ~PipelineGraphics();
  PipelineGraphics(PipelineGraphics const &) = delete;
  PipelineGraphics &operator=(PipelineGraphics const &) = delete;
  PipelineGraphics(PipelineGraphics &&) = delete;
  PipelineGraphics &operator=(PipelineGraphics &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkPipeline Handle();

private:
  [[nodiscard]] VkPipeline Create(const VkGraphicsPipelineCreateInfo &create_info) const;
  // Reference to resource this object is created with
  VkDevice const context_device_;
  VkPipeline const pipeline_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif