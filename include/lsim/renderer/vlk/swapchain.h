//
// Created by Ivars Rusbergs in 2021
//
// General purpose of the swap chain is to synchronize the presentation of
// images with the refresh rate of the screen.
#ifndef LSIM_RENDERER_VLK_SWAPCHAIN_H_
#define LSIM_RENDERER_VLK_SWAPCHAIN_H_
#include <vector>

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
            const VkSurfaceKHR &surface);
  ~Swapchain();

private:
  VkSurfaceFormatKHR
  SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) const;
  // Reference to object this resource was created with
  const VkDevice &device_;
  const SupportDetails support_details_;
  // NOTE: should be initialized before vulkan swapchain object
  // Selected format of swapchain images
  const VkSurfaceFormatKHR surface_format_;
};
} // namespace lsim::renderer::vlk
#endif