#include "swapchain.h"
#include <stdexcept>
#include <algorithm>
#include <base/log.h>

namespace renderer::vlk {
Swapchain::Swapchain(const Device& device, const VkSurfaceKHR& surface, GLFWwindow* window) :
    device_(device.Get()),
    swapchain_(Create(device.GetGPU(),device.GetQueue().GetFamilyIndices(), surface, window)),
    images_(GetImages()),
    image_views_(CreateImageViews(images_)) {
    base::Log::Info("Renderer: swapchain created");
} 

Swapchain::~Swapchain() {
    base::Log::Info("Renderer: swapchain destroying...");
    for (auto image_view : image_views_) {
        vkDestroyImageView(device_, image_view, nullptr);
    }
    vkDestroySwapchainKHR(device_, swapchain_, nullptr);
}

const VkSwapchainKHR& Swapchain::Get() const {
    return swapchain_;
}

const VkExtent2D& Swapchain::GetExtent() const {
    return extent_;
}

const VkSurfaceFormatKHR& Swapchain::GetSurfaceFormat() const {
    return surface_format_;
}

const std::vector<VkImageView>& Swapchain::GetImageViews() const {
    return image_views_;
}

// Next image index from swapchain that is available
// image_available_semaphore - a semaphore that will become signaled when the presentation
// engine has released ownership of the image
uint32_t Swapchain::AcquireNextImageIndex(const VkSemaphore& image_available_semaphore) const {
    uint32_t image_index;
    constexpr uint64_t unlimited_timeout = std::numeric_limits<uint64_t>::max();
    ErrorCheck(vkAcquireNextImageKHR(device_, swapchain_, unlimited_timeout, image_available_semaphore, 
        VK_NULL_HANDLE, &image_index));
    return image_index;
}

VkSwapchainKHR Swapchain::Create(const VkPhysicalDevice& gpu, const DeviceQueue::FamilyIndices& qf_indices, 
    const VkSurfaceKHR& surface, GLFWwindow* window) {
    surface_format_ = SelectSurfaceFormat(gpu, surface);
    const VkPresentModeKHR present_mode = SelectPresentMode(gpu, surface);
    extent_ = RetrieveExtent(gpu, surface, window);
    uint32_t image_count = SelectImageCount(gpu, surface);
    VkSwapchainCreateInfoKHR create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format_.format;
    create_info.imageColorSpace = surface_format_.colorSpace;
    create_info.imageExtent = extent_;
    create_info.imageArrayLayers = 1;
    // Other usages than 'color attachment' should be checked first because they are not guaranteed
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    // Handle swapchain iamges in case graphics and presentation queues differ
    if (!qf_indices.IsGraphicsPresentMatch()) {
        // Used by multiple queues that is why need to specify which
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = std::vector<uint32_t>({ qf_indices.graphics.value(),
                                                                    qf_indices.present.value() }).data();
    }
    else {
        // Image is owned by only one queue at a time
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0; // Optional
        create_info.pQueueFamilyIndices = nullptr; // Optional
    }
    // No need to use different transforms on images
    create_info.preTransform = SelectTransform(gpu, surface);
    // Ignore blending with other windows
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    // We dont care about colors of pixels that are obscured (ie. by antoher window)
    create_info.clipped = VK_TRUE;
    // When swapchain needs to be recreated (resizig window) this field should be set to old swachain
    create_info.oldSwapchain = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain;
    ErrorCheck(vkCreateSwapchainKHR(device_, &create_info, nullptr, &swapchain));
    return swapchain;
}

VkSurfaceFormatKHR Swapchain::SelectSurfaceFormat(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const {
    // Preferred formats that we look for
    const auto preferred_color_format = VK_FORMAT_B8G8R8A8_UNORM;
    const auto preferred_color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, nullptr);
    if (format_count == 0) {
        throw std::runtime_error("Renderer: no available surface formats");
    }
    std::vector<VkSurfaceFormatKHR> available_formats(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, available_formats.data());
    // If surface has no preferred format we return our desired format (best case scenario)
    if (available_formats.size() == 1 && available_formats.at(0).format == VK_FORMAT_UNDEFINED) {
        return { preferred_color_format, preferred_color_space };
    }
    // If we are not free to choose any format, see if our desired format is available
    for (const auto& format : available_formats) {
        if (format.format == preferred_color_format && format.colorSpace == preferred_color_space) {
            return format;
        }
    }
    // If everything failed return first available format 
    return available_formats.at(0);
}

VkPresentModeKHR Swapchain::SelectPresentMode(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const {
    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, nullptr);
    std::vector<VkPresentModeKHR> available_present_modes(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &present_mode_count, available_present_modes.data());
    // Fifo is guaranteed to be present, but some drivers dont properly support it
    // so we choose between mailbox (tripple buffering) and imidiate
    auto preferred_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto& present_mode : available_present_modes) {
        // This is the best case so we return imidiatly if encountered
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return present_mode;
        }
        else if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            preferred_mode = present_mode;
        }
    }
    return preferred_mode;
}

VkExtent2D Swapchain::RetrieveExtent(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface,
    GLFWwindow* window) const {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities);
    VkExtent2D extent;
    // Most of the time extent matches surface resolution
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        extent = capabilities.currentExtent;
    }
    // But in some OS winows size is determined by extent so we need to set it between min and max
    else {
        // Instead of passing dimensions we retrieve these values from framebuffer
        // Could also retrieve from window but not safe because window dimensions and framebuffer may not match
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        VkExtent2D actual_extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        //actual_extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actual_extent.width));
        //actual_extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actual_extent.height));
        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        extent = actual_extent;
    }
    return extent;
}

// Select number of images in the swapchain
// NOTE: swapchain is allowed to create more images than we specify
uint32_t Swapchain::SelectImageCount(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities);
    // It is recomended to request one image than the minimum for driver to work more optimal
    uint32_t image_count = capabilities.minImageCount + 1;
    // Make sure not to exceed maximum allowed image count (0 means no maximum)
    if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount) {
        image_count = capabilities.maxImageCount;
    }
    return image_count;
}

VkSurfaceTransformFlagBitsKHR Swapchain::SelectTransform(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities);
    // No need to transform
    return capabilities.currentTransform;
}

// Images are created together with swapchain so we only retrieve them 
std::vector<VkImage> Swapchain::GetImages() const {
    // Get handles to swapchain images
    // NOTE: swapchain is allowed to create more images than we specify
    uint32_t image_count;
    vkGetSwapchainImagesKHR(device_, swapchain_, &image_count, nullptr);
    std::vector<VkImage> swapchain_images(image_count);
    ErrorCheck(vkGetSwapchainImagesKHR(device_, swapchain_, &image_count, swapchain_images.data()));
    return swapchain_images;
}

// Images can be accessed through image view
std::vector<VkImageView> Swapchain::CreateImageViews(const std::vector<VkImage>& images) const {
    std::vector<VkImageView> image_views(images_.size());
    uint32_t i = 0;
    for (const auto& image : images_) {
        VkImageViewCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = image;
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = surface_format_.format;
        // Color values coming out from shader is mapped on-to-one
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;
        ErrorCheck(vkCreateImageView(device_, &create_info, nullptr, &image_views[i++]));
    }
    return image_views;
}
}; // renderer vlk