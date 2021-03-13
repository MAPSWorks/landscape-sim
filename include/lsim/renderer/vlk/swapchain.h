//
// Created by Ivars Rusbergs in 2021
//
// General purpose of the swap chain is to synchronize the presentation of
// images with the refresh rate of the screen.
#ifndef LSIM_RENDERER_VLK_SWAPCHAIN_H_
#define LSIM_RENDERER_VLK_SWAPCHAIN_H_
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

#include "image_view.h"
#include "queue_families.h"

namespace lsim::renderer::vlk {
class Swapchain {
public:
  // Describes swapchain support capabilities
  struct SupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
    // If swapchain support is enaugh for the engine
    [[nodiscard]] bool IsCapable() const {
      return !formats.empty() && !present_modes.empty();
    }
  };
  // Gets and returns swapchain support details from given physical device and
  // surface
  static SupportDetails QuerySupport(const VkPhysicalDevice &gpu,
                                     const VkSurfaceKHR &surface);
  Swapchain(VkDevice device, const VkPhysicalDevice &gpu,
            const VkSurfaceKHR &surface, const QueueFamilies &qf_indices,
            SDL_Window *window);
  ~Swapchain();
  Swapchain(Swapchain const &) = delete;
  Swapchain &operator=(Swapchain const &) = delete;
  Swapchain(Swapchain &&) = delete;
  Swapchain &operator=(Swapchain &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkSwapchainKHR Handle() const;
  [[nodiscard]] const VkExtent2D &Extent() const;
  [[nodiscard]] const VkSurfaceFormatKHR &SurfaceFormat() const;
  [[nodiscard]] const std::vector<std::unique_ptr<ImageView>> &ImageViews();
  // Return next available image index from the swapchain
  [[nodiscard]] uint32_t
  AcquireNextImageIndex(const VkSemaphore &image_available_sem) const;

private:
  VkSwapchainKHR Create(const VkSurfaceKHR &surface,
                        const QueueFamilies &qf_indices,
                        const VkSurfaceCapabilitiesKHR &caps);
  // Retrieve list of images from swapchain
  [[nodiscard]] std::vector<VkImage> RetrieveImages() const;
  // Create image views for given images
  [[nodiscard]] std::vector<std::unique_ptr<ImageView>>
  CreateImageViews(const std::vector<VkImage> &images) const;
  // Pointer to object this resource was created with
  VkDevice const context_device_;
  // NOTE: should be initialized before vulkan swapchain object
  // Selected format of swapchain images
  const SupportDetails support_details_;
  const VkSurfaceFormatKHR surface_format_;
  const VkPresentModeKHR present_mode_;
  // It ususlly is the same size as surface, but now always, see
  // RetrieveExtent()
  const VkExtent2D extent_;
  VkSwapchainKHR const swapchain_ = VK_NULL_HANDLE;
  // List of image handles acquired from swapchain
  // Number of images should be rrtrieved from this array if needed
  const std::vector<VkImage> images_;
  // Image views for swapchain images
  const std::vector<std::unique_ptr<ImageView>> image_views_;
};
} // namespace lsim::renderer::vlk
#endif