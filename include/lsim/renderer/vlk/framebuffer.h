//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan framebuffer object.
#ifndef LSIM_RENDERER_VLK_FRAMEBUFFER_H_
#define LSIM_RENDERER_VLK_FRAMEBUFFER_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class Framebuffer {
public:
  Framebuffer(VkDevice device, VkRenderPass render_pass,
              VkImageView swapchain_image_view,
              const VkExtent2D &swapchain_extent, VkImageView depth_image_view);
  ~Framebuffer();
  Framebuffer(Framebuffer const &) = delete;
  Framebuffer &operator=(Framebuffer const &) = delete;
  // Framebuffer(Framebuffer const &) = delete;
  // Framebuffer operator=(Framebuffer const &) = delete;
  // Move constructor
  Framebuffer(Framebuffer &&other) noexcept;
  // Move asignment
  Framebuffer &operator=(Framebuffer &&other) noexcept;
  // Returns Vulkan object handle
  [[nodiscard]] VkFramebuffer Handle();

private:
  void Destroy();
  [[nodiscard]] VkFramebuffer Create(VkRenderPass render_pass,
                                     VkImageView swapchain_image_view,
                                     const VkExtent2D &swapchain_extent,
                                     VkImageView depth_image_view) const;
  // Pointer to resource this object is created with
  VkDevice const context_device_;
  VkFramebuffer framebuffer_;
};
} // namespace lsim::renderer::vlk
#endif