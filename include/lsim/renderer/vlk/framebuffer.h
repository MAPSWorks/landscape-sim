//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan framebuffer layout object.
#ifndef LSIM_RENDERER_VLK_FRAMEBUFFER_H_
#define LSIM_RENDERER_VLK_FRAMEBUFFER_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class Framebuffer {
public:
  Framebuffer(const VkDevice &device, const VkRenderPass &render_pass,
              const VkImageView &swapchain_image_view,
              const VkExtent2D &swapchain_extent,
              const VkImageView &depth_image_view);
  ~Framebuffer();
  //Framebuffer(Framebuffer const &) = delete;
  //Framebuffer operator=(Framebuffer const &) = delete;
  // Move constructor
  Framebuffer(Framebuffer &&other) noexcept;
  // Move asignment
  Framebuffer &operator=(Framebuffer &&other) noexcept;
  const VkFramebuffer &Get() const;

private:
  void Destroy();
  VkFramebuffer Create(const VkRenderPass &render_pass,
                       const VkImageView &swapchain_image_view,
                       const VkExtent2D &swapchain_extent,
                       const VkImageView &depth_image_view) const;
  // Reference to resource this object is created with
  const VkDevice &device_;
  VkFramebuffer framebuffer_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk
#endif