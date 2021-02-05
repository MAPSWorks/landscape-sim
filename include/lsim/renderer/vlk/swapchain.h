//
// Created by Ivars Rusbergs in 2021
//
// General purpose of the swap chain is to synchronize the presentation of
// images with the refresh rate of the screen.
#ifndef LSIM_RENDERER_VLK_SWAPCHAIN_H_
#define LSIM_RENDERER_VLK_SWAPCHAIN_H_
#include <vector>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

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
            const VkSurfaceKHR &surface, SDL_Window *window);
  ~Swapchain();

private:
  VkSurfaceFormatKHR
  SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) const;
  VkPresentModeKHR
  SelectPresentMode(const std::vector<VkPresentModeKHR> &modes) const;
  VkExtent2D RetrieveExtent(const VkSurfaceCapabilitiesKHR &caps,
                            SDL_Window *window) const;
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
};
} // namespace lsim::renderer::vlk
#endif