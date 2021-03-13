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
#include "lsim/renderer/vlk/queue_families.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
// Internal linkage
namespace {
VkSurfaceFormatKHR
SelectSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) {
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

VkPresentModeKHR SelectPresentMode(const std::vector<VkPresentModeKHR> &modes) {
  // Fifo is guaranteed to be present, but some drivers dont properly support it
  // so we choose between mailbox (tripple buffering) and imidiate
  auto preferred_mode = VK_PRESENT_MODE_FIFO_KHR;
  for (const auto &present_mode : modes) {
    // This is the best case so we return imidiatly if encountered
    if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      preferred_mode =  present_mode;
      break;
    }
    if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
      preferred_mode = present_mode;
    }
  }
  return preferred_mode;
}

VkExtent2D RetrieveWindowExtent(const VkSurfaceCapabilitiesKHR &caps,
                                SDL_Window *window) {
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
    int width = 0;
    int height = 0;
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

// Select number of images in the swapchain
// Swapchain is allowed to create more images than we specify
uint32_t SelectImageCount(const VkSurfaceCapabilitiesKHR &caps) {
  // It is recomended to request one image than the minimum for driver to work
  // more optimal
  uint32_t image_count = caps.minImageCount + 1;
  // Make sure not to exceed maximum allowed image count (0 means no maximum)
  if (caps.maxImageCount > 0 && image_count > caps.maxImageCount) {
    image_count = caps.maxImageCount;
  }
  return image_count;
}

} // namespace

// static
Swapchain::SupportDetails Swapchain::QuerySupport(const VkPhysicalDevice &gpu,
                                                  const VkSurfaceKHR &surface) {
  SupportDetails details;
  // Capabilities
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface,
                                            &details.capabilities);
  // Formats
  uint32_t format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr);
  if (format_count != 0) {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count,
                                         details.formats.data());
  }
  // Present modes
  uint32_t present_mode_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count,
                                            nullptr);
  if (present_mode_count != 0) {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count,
                                              details.present_modes.data());
  }

  return details;
}

Swapchain::Swapchain(VkDevice device, const VkPhysicalDevice &gpu,
                     const VkSurfaceKHR &surface,
                     const QueueFamilies &qf_indices, SDL_Window *window)
    : context_device_(device), support_details_(QuerySupport(gpu, surface)),
      surface_format_(SelectSurfaceFormat(support_details_.formats)),
      present_mode_(SelectPresentMode(support_details_.present_modes)),
      extent_(RetrieveWindowExtent(support_details_.capabilities, window)),
      swapchain_(Create(surface, qf_indices, support_details_.capabilities)),
      images_(RetrieveImages()), image_views_(CreateImageViews(images_)) {

  base::Log::Info("renderer", "swapchain", "created");
}

Swapchain::~Swapchain() {
  base::Log::Info("renderer", "swapchain", "destroying..");
  vkDestroySwapchainKHR(context_device_, swapchain_, nullptr);
}

VkSwapchainKHR Swapchain::Handle() const { return swapchain_; }

const VkExtent2D &Swapchain::Extent() const { return extent_; }

const VkSurfaceFormatKHR &Swapchain::SurfaceFormat() const {
  return surface_format_;
}

std::vector<ImageView> &Swapchain::ImageViews() {
  return image_views_;
}

// image_available_semaphore - a semaphore that will become signaled when the
// presentation engine has released ownership of the image
uint32_t
Swapchain::AcquireNextImageIndex(const VkSemaphore &image_available_sem) const {
  uint32_t image_index = 0;
  constexpr auto unlimited_timeout = std::numeric_limits<uint64_t>::max();
  ErrorCheck(vkAcquireNextImageKHR(context_device_, swapchain_, unlimited_timeout,
                                   image_available_sem, VK_NULL_HANDLE,
                                   &image_index));
  return image_index;
}

VkSwapchainKHR Swapchain::Create(const VkSurfaceKHR &surface,
                                 const QueueFamilies &qf_indices,
                                 const VkSurfaceCapabilitiesKHR &caps) {
  const auto min_image_count = SelectImageCount(caps);
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
  if (!qf_indices.GraphicsPresentMatch()) {
    // Used by multiple queues that is why need to specify which
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices =
        std::vector<uint32_t>({qf_indices.Graphics(), qf_indices.Present()})
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
  VkSwapchainKHR swapchain = VK_NULL_HANDLE;
  ErrorCheck(vkCreateSwapchainKHR(context_device_, &create_info, nullptr, &swapchain));
  return swapchain;
}

// Images are created together with swapchain so we only retrieve them
std::vector<VkImage> Swapchain::RetrieveImages() const {
  // Get handles to swapchain images
  // NOTE: swapchain is allowed to create more images than we specify
  uint32_t image_count = 0;
  vkGetSwapchainImagesKHR(context_device_, swapchain_, &image_count, nullptr);
  std::vector<VkImage> swapchain_images(image_count);
  ErrorCheck(vkGetSwapchainImagesKHR(context_device_, swapchain_, &image_count,
                                     swapchain_images.data()));
  return swapchain_images;
}

std::vector<ImageView>
Swapchain::CreateImageViews(const std::vector<VkImage> &images) const {
  std::vector<ImageView> image_views;
  // Number of image views and images should match (also their order)!
  for (const auto &image : images) {
    image_views.emplace_back(context_device_, image, surface_format_.format);
  }
  return image_views;
}

} // namespace lsim::renderer::vlk