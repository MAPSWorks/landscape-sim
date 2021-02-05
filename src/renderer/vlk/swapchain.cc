//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/swapchain.h"

#include <vector>

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

Swapchain::Swapchain(const VkDevice &device, const VkPhysicalDevice &gpu,
                     const VkSurfaceKHR &surface)
    : device_(device), support_details_(QuerySupport(gpu, surface)), 
    surface_format_(SelectSurfaceFormat(support_details_.formats)) {}
Swapchain::~Swapchain() {}

VkSurfaceFormatKHR Swapchain::SelectSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &formats) const {
  // Preferred formats that we look for
  const auto preferred_color_format = VK_FORMAT_B8G8R8A8_UNORM;
  const auto preferred_color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  // If surface has no preferred format we return our desired format (best case
  // scenario)
  if (formats.size() == 1 && formats.at(0).format == VK_FORMAT_UNDEFINED) {
    return {preferred_color_format, preferred_color_space};
  }
  // If we are not free to choose any format, see if our desired format is
  // available
  for (const auto &format : formats) {
    if (format.format == preferred_color_format &&
        format.colorSpace == preferred_color_space) {
      return format;
    }
  }
  // If everything failed return first available format
  return formats.at(0);
}

} // namespace lsim::renderer::vlk