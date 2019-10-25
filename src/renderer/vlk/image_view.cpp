#include "image_view.h"
#include <base/log.h>

namespace renderer::vlk {
ImageView::ImageView(const VkDevice& device, const VkImage& image, VkFormat format) : 
    device_(device),
    image_view_(Create(image, format)) {
    base::Log::Info("Renderer: image view created");
}

ImageView::~ImageView() {
    base::Log::Info("Renderer: image view destroying...");
    vkDestroyImageView(device_, image_view_, nullptr);
}

const VkImageView& ImageView::Get() const {
    return image_view_;
}

VkImageView ImageView::Create(const VkImage& image, VkFormat format) const {
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
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;
    VkImageView image_view;
    ErrorCheck(vkCreateImageView(device_, &create_info, nullptr, &image_view));
    return image_view;
}

};  // renderer vlk
