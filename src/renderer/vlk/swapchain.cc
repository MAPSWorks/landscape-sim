//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/swapchain.h"

#include <algorithm>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
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
                     const VkSurfaceKHR &surface, SDL_Window *window)
    : device_(device), support_details_(QuerySupport(gpu, surface)),
      surface_format_(SelectSurfaceFormat(support_details_.formats)),
      present_mode_(SelectPresentMode(support_details_.present_modes)),
      extent_(RetrieveExtent(support_details_.capabilities, window)) {}

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

VkPresentModeKHR
Swapchain::SelectPresentMode(const std::vector<VkPresentModeKHR> &modes) const {
  // Fifo is guaranteed to be present, but some drivers dont properly support it
  // so we choose between mailbox (tripple buffering) and imidiate
  auto preferred_mode = VK_PRESENT_MODE_FIFO_KHR;
  for (const auto &present_mode : modes) {
    // This is the best case so we return imidiatly if encountered
    if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return present_mode;
    } else if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
      preferred_mode = present_mode;
    }
  }
  return preferred_mode;
}

VkExtent2D Swapchain::RetrieveExtent(const VkSurfaceCapabilitiesKHR &caps,
                                     SDL_Window *window) const {

  VkExtent2D extent;
  // Most of the time extent matches surface resolution
  if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    extent = caps.currentExtent;
  }
  // But in some OS winows size is determined by extent so we need to set it
  // between min and max.
  else {
    // Instead of passing dimensions we retrieve these values from framebuffer
    // Could also retrieve from window but not safe because window dimensions
    // and framebuffer may not match.
    int width, height;
    SDL_Vulkan_GetDrawableSize(window, &width, &height);
    VkExtent2D actual_extent = {static_cast<uint32_t>(width),
                                static_cast<uint32_t>(height)};
    actual_extent.width =
        std::clamp(actual_extent.width, caps.minImageExtent.width,
                   caps.maxImageExtent.width);
    actual_extent.height =
        std::clamp(actual_extent.height, caps.minImageExtent.height,
                   caps.maxImageExtent.height);
    extent = actual_extent;
  }
  return extent;
}

} // namespace lsim::renderer::vlk