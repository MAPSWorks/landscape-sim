//
// Created by Ivars Rusbergs in 2021
//
// General purpose of the swap chain is to synchronize the presentation of
// images with the refresh rate of the screen.
#ifndef LSIM_RENDERER_VLK_SWAPCHAIN_H_
#define LSIM_RENDERER_VLK_SWAPCHAIN_H_
#include <bits/stdint-uintn.h>
#include <vector>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

#include "device_queue.h"
#include "image_view.h"

namespace lsim::renderer::vlk {
class Swapchain {
public:
  // Describes swapchain support capabilities
  struct SupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
    // If swapchain support is enaugh for the engine
    bool IsCapable() const {
      return !formats.empty() && !present_modes.empty();
    }
  };
  // Gets and returns swapchain support details from given physical device and
  // surface
  static SupportDetails QuerySupport(const VkPhysicalDevice &gpu,
                                     const VkSurfaceKHR &surface);
  Swapchain(const VkDevice &device, const VkPhysicalDevice &gpu,
            const VkSurfaceKHR &surface,
            const DeviceQueue::FamilyIndices &qf_indices, SDL_Window *window);
  ~Swapchain();
  Swapchain(Swapchain const &) = delete;
  Swapchain operator=(Swapchain const &) = delete;
  // Returns Vulkan object handle
  const VkSwapchainKHR &Handle() const;
  const VkExtent2D &Extent() const;
  const VkSurfaceFormatKHR &SurfaceFormat() const;
  const std::vector<ImageView> &ImageViews() const;
  // Return next available image index from the swapchain
  uint32_t AcquireNextImageIndex(const VkSemaphore &image_available_sem) const;

private:
  VkSwapchainKHR Create(const VkSurfaceKHR &surface,
                        const DeviceQueue::FamilyIndices &qf_indices,
                        const VkSurfaceCapabilitiesKHR &caps);
  VkSurfaceFormatKHR
  SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) const;
  VkPresentModeKHR
  SelectPresentMode(const std::vector<VkPresentModeKHR> &modes) const;
  VkExtent2D RetrieveExtent(const VkSurfaceCapabilitiesKHR &caps,
                            SDL_Window *window) const;
  // Select number of images in the swapchain
  uint32_t SelectImageCount(const VkSurfaceCapabilitiesKHR &caps) const;
  // Retrieve list of images from swapchain
  std::vector<VkImage> RetrieveImages() const;
  // Create image views for given images
  std::vector<ImageView>
  CreateImageViews(const std::vector<VkImage> &images) const;
  // Reference to object this resource was created with
  const VkDevice &device_;
  // NOTE: should be initialized before vulkan swapchain object
  // Selected format of swapchain images
  const SupportDetails support_details_;
  const VkSurfaceFormatKHR surface_format_;
  const VkPresentModeKHR present_mode_;
  // It ususlly is the same size as surface, but now always, see
  // RetrieveExtent()
  const VkExtent2D extent_;
  const VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
  // List of image handles acquired from swapchain
  // Number of images should be rrtrieved from this array if needed
  const std::vector<VkImage> images_;
  // Image views for swapchain images
  const std::vector<ImageView> image_views_;
};
} // namespace lsim::renderer::vlk
#endif