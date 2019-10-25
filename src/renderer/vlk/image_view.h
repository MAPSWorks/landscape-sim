#pragma once
#include "vulkan_shared.h"

// Image view wrapper
namespace renderer::vlk {
class ImageView {
public:
    // Create image view for given image with given format
    ImageView(const VkDevice& device, const VkImage& image, VkFormat format);
    ~ImageView();
    ImageView(ImageView const&) = delete;
    ImageView operator=(ImageView const&) = delete;
    const VkImageView& Get() const;
private:
    VkImageView Create(const VkImage& image, VkFormat format) const;
    // Reference to resource this resource is created with
    const VkDevice& device_;
    const VkImageView image_view_ = VK_NULL_HANDLE;
};
}; // renderer vlk