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
#include "lsim/renderer/vlk/device_queue.h"
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
                     const VkSurfaceKHR &surface,
                     const DeviceQueue::FamilyIndices &qf_indices,
                     SDL_Window *window)
    : device_(device), support_details_(QuerySupport(gpu, surface)),
      surface_format_(SelectSurfaceFormat(support_details_.formats)),
      present_mode_(SelectPresentMode(support_details_.present_modes)),
      extent_(RetrieveExtent(support_details_.capabilities, window)),
      swapchain_(Create(surface, qf_indices)) {}

Swapchain::~Swapchain() {}

const VkSwapchainKHR &Swapchain::Get() const { return swapchain_; }

VkSwapchainKHR Swapchain::Create(const VkSurfaceKHR &surface,
                                 const DeviceQueue::FamilyIndices &qf_indices) {
  const auto min_image_count = SelectImageCount();
  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = surface;
  create_info.minImageCount = min_image_count;
  create_info.imageFormat = surface_format_.format;
  create_info.imageColorSpace = surface_format_.colorSpace;
  create_info.imageExtent = extent_;
  create_info.imageArrayLayers = 1;
  // Other usages than 'color attachment' should be checked first because they
  // are not guaranteed
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  // Handle swapchain iamges in case graphics and presentation queues differ
  if (!qf_indices.IsGraphicsPresentMatch()) {
    // Used by multiple queues that is why need to specify which
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices =
        std::vector<uint32_t>(
            {qf_indices.graphics.value(), qf_indices.present.value()})
            .data();
  } else {
    // Image is owned by only one queue at a time
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0;     // Optional
    create_info.pQueueFamilyIndices = nullptr; // Optional
  }
  // No need to use different transforms on images
  create_info.preTransform = support_details_.capabilities.currentTransform;
  // Ignore blending with other windows
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode_;
  // We dont care about colors of pixels that are obscured (ie. by antoher
  // window)
  create_info.clipped = VK_TRUE;
  // When swapchain needs to be recreated (resizig window) this field should be
  // set to old swachain
  create_info.oldSwapchain = VK_NULL_HANDLE;
  VkSwapchainKHR swapchain;
  ErrorCheck(vkCreateSwapchainKHR(device_, &create_info, nullptr, &swapchain));
  return swapchain;
}

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

// Swapchain is allowed to create more images than we specify
uint32_t Swapchain::SelectImageCount() const {
  const auto capabilities = support_details_.capabilities;
  // It is recomended to request one image than the minimum for driver to work
  // more optimal
  uint32_t image_count = capabilities.minImageCount + 1;
  // Make sure not to exceed maximum allowed image count (0 means no maximum)
  if (capabilities.maxImageCount > 0 &&
      image_count > capabilities.maxImageCount) {
    image_count = capabilities.maxImageCount;
  }
  return image_count;
}

} // namespace lsim::renderer::vlk