//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/swapchain.h"

#include <vulkan/vulkan.h>

#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
// static
Swapchain::SupportDetails Swapchain::QuerySupport(const VkPhysicalDevice &gpu,
                                                  const VkSurfaceKHR &surface) {
  SupportDetails details;
  // Capabilities
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface,
                                                       &details.capabilities);
  // Formats
  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr);
  if (format_count != 0) {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count,
                                         details.formats.data());
  }
  // Present modes
  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count,
                                            nullptr);
  if (present_mode_count != 0) {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count,
                                              details.present_modes.data());
  }

  return details;
}

} // namespace lsim::renderer::vlk