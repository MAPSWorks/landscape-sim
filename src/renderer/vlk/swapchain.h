#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "vulkan_shared.h"
#include "device.h"

namespace renderer::vlk {
class Swapchain {
public:
    Swapchain(const Device &device, const VkSurfaceKHR &surface, GLFWwindow* window);
    ~Swapchain();
    Swapchain(Swapchain const&) = delete;
    Swapchain operator=(Swapchain const&) = delete;
    const VkExtent2D& GetExtent() const;
    const VkSurfaceFormatKHR& GetSurfaceFormat() const;
private:
    VkSwapchainKHR Create(const Device& device, const VkSurfaceKHR& surface,  GLFWwindow* window);
    VkSurfaceFormatKHR SelectSurfaceFormat(const VkPhysicalDevice & gpu, const VkSurfaceKHR& surface) const;
    VkPresentModeKHR SelectPresentMode(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const;
    VkExtent2D RetrieveExtent(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface, GLFWwindow* window) const;
    // Select number of images in the swapchain
    uint32_t SelectImageCount(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const;
    // Select transformation of images in swapchain
    VkSurfaceTransformFlagBitsKHR SelectTransform(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const;
    // Retrieve list of images from swapchain
    std::vector<VkImage> GetImages(const VkDevice& device) const;
    std::vector<VkImageView> CreateImageViews(const VkDevice& device, const std::vector<VkImage> &images) const;
    // We grab reference and store it here because we need it in destructor
    const VkDevice& device_;
    // Parameters below is not constant because they can probably change
    // NOTE: These params should be above swapchain_, because they get initialized 
    // right before swapchain, otherwise they are reset to 0 after swapchain initialization
    // Dimension of swapchain images
    // It ususlly is the same size as surface, but now always, see RetrieveExtent()
    VkExtent2D extent_ = {};
    // Selected format of swapchain images
    VkSurfaceFormatKHR surface_format_ = {};
    const VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
    // List of images acquired from swapchain
    // Number of images should be rwtrieved from this array if needed
    const std::vector<VkImage> images_;
    const std::vector<VkImageView> image_views_;
};
}; // renderer vlk