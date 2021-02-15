//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan render pass object.
#ifndef LSIM_RENDERER_VLK_RENDER_PASS_H_
#define LSIM_RENDERER_VLK_RENDER_PASS_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Description of a render pass as well as attachments
// and subpasses
class RenderPass {
public:
  RenderPass(const VkDevice &device, const VkFormat &swapchain_format,
             const VkFormat &depth_format);
  ~RenderPass();
  RenderPass(RenderPass const &) = delete;
  RenderPass operator=(RenderPass const &) = delete;
  // Returns Vulkan object handle
  const VkRenderPass &Handle() const;

private:
  VkRenderPass Create(const VkFormat &swapchain_format,
                      const VkFormat &depth_format) const;
  // Reference to object this resource was created with
  const VkDevice &device_;
  const VkRenderPass render_pass_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk
#endif