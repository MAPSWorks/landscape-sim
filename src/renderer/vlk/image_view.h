#pragma once
#include "vulkan_shared.h"

// Image view wrapper
namespace renderer::vlk {
class ImageView {
public:
    // Create image view for given image with given format
    // Specify also aspect flag (color (default), depth ...)
    ImageView(const VkDevice& device, const VkImage& image, VkFormat format, VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT);
    ~ImageView();
    ImageView(ImageView const&) = delete;
    ImageView operator=(ImageView const&) = delete;
    // Move constructor
    ImageView(ImageView&& other) noexcept;
    // Move asignment
    ImageView& operator=(ImageView&& other) noexcept;
    const VkImageView& Get() const;
private:
    VkImageView Create(const VkImage& image, VkFormat format, VkImageAspectFlags aspect_flags) const;
    void Destroy();
    // Reference to resource this resource is created with
    const VkDevice& device_;
    VkImageView image_view_ = VK_NULL_HANDLE;
};
}; // renderer vlk