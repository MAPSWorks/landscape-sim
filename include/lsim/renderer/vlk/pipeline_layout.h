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
  PipelineLayout(VkDevice device,
                 const VkPipelineLayoutCreateInfo &create_info);
  ~PipelineLayout();
  PipelineLayout(PipelineLayout const &) = delete;
  PipelineLayout &operator=(PipelineLayout const &) = delete;
  PipelineLayout(PipelineLayout &&) = delete;
  PipelineLayout &operator=(PipelineLayout &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkPipelineLayout Handle() const;

private:
  [[nodiscard]] VkPipelineLayout Create(const VkPipelineLayoutCreateInfo &create_info) const;
  // Pointer to object this resource was created with
  VkDevice const context_device_;
  VkPipelineLayout const pipeline_layout_ = VK_NULL_HANDLE;
};

} // namespace lsim::renderer::vlk
#endif