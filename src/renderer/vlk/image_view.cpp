#include "image_view.h"
#include <base/log.h>

namespace renderer::vlk {
ImageView::ImageView(const VkDevice& device, const VkImage& image, VkFormat format, VkImageAspectFlags aspect_flags) :
    device_(device),
    image_view_(Create(image, format, aspect_flags)) {
    base::Log::Info("Renderer: image view created");
}

ImageView::~ImageView() {
    Destroy();
}

ImageView::ImageView(ImageView&& other) noexcept : 
    device_(other.device_),
    image_view_(VK_NULL_HANDLE) {
    // Call move-asignment operator
    *this = std::move(other);
}

ImageView& ImageView::operator=(ImageView&& other) noexcept {
    // Check self-move
    if (this != &other) {
        // Free this resource if initialized
        Destroy();
        // Change handle ownership
        this->image_view_ = other.image_view_;
        // Remove handle from about-to-deleted object so it doesnt destroy resource
        other.image_view_ = VK_NULL_HANDLE;
    }
    return *this;
}

const VkImageView& ImageView::Get() const {
    return image_view_;
}

VkImageView ImageView::Create(const VkImage& image, VkFormat format, VkImageAspectFlags aspect_flags) const {
    VkImageViewCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = image;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;
    // Color values coming out from shader is mapped on-to-one
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = aspect_flags;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;
    VkImageView image_view;
    ErrorCheck(vkCreateImageView(device_, &create_info, nullptr, &image_view));
    return image_view;
}

// Properly destroy resource
void ImageView::Destroy() {
    // Handle must be in valid state
    if (image_view_ != VK_NULL_HANDLE) {
        base::Log::Info("Renderer: image view destroying...");
        vkDestroyImageView(device_, image_view_, nullptr);
        // Make sure handle is not valid anymore
        image_view_ = VK_NULL_HANDLE;
    }
}

};  // renderer vlk
