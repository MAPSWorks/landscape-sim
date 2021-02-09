//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan pipeline layout object.
#ifndef LSIM_RENDERER_VLK_PIPELINE_LAYOUT_H_
#define LSIM_RENDERER_VLK_PIPELINE_LAYOUT_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Describes shader resource layout in pipeline.
// This object is needed when creating pipeline and when binding descriptor set
// to command buffer (to establish compatability).
class PipelineLayout {
public:
  PipelineLayout(const VkDevice &device,
                 const VkPipelineLayoutCreateInfo &create_info);
  ~PipelineLayout();
  PipelineLayout(PipelineLayout const &) = delete;
  PipelineLayout operator=(PipelineLayout const &) = delete;
  const VkPipelineLayout &Get() const;

private:
  VkPipelineLayout Create(const VkPipelineLayoutCreateInfo &create_info) const;
  // Reference to object this resource was created with
  const VkDevice &device_;
  const VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
};

} // namespace lsim::renderer::vlk
#endif