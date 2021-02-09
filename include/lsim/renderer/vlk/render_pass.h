//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan render pass object.
#ifndef LSIM_RENDERER_VLK_PIPELINE_LAYOUT_H_
#define LSIM_RENDERER_VLK_PIPELINE_LAYOUT_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class RenderPass {
public:
  RenderPass(const VkDevice &device, const VkFormat &swapchain_format,
             const VkFormat &depth_format);
  ~RenderPass();
  RenderPass(RenderPass const &) = delete;
  RenderPass operator=(RenderPass const &) = delete;
  const VkRenderPass &Get() const;

private:
  VkRenderPass Create(const VkFormat &swapchain_format,
                      const VkFormat &depth_format) const;
  // Reference to object this resource was created with
  const VkDevice &device_;
  const VkRenderPass render_pass_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk
#endif