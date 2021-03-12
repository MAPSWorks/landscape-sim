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
  RenderPass(VkDevice device, const VkFormat &swapchain_format,
             const VkFormat &depth_format);
  ~RenderPass();
  RenderPass(RenderPass const &) = delete;
  RenderPass &operator=(RenderPass const &) = delete;
  RenderPass(RenderPass &&) = delete;
  RenderPass &operator=(RenderPass &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkRenderPass Handle() const;

private:
  [[nodiscard]] VkRenderPass Create(const VkFormat &swapchain_format,
                                    const VkFormat &depth_format) const;
  // Pointer to object this resource was created with
  VkDevice const context_device_;
  VkRenderPass const render_pass_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk
#endif