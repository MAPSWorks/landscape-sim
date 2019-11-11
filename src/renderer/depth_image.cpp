#include "depth_image.h"
#include <base/log.h>

namespace renderer {
DepthImage::DepthImage(const VkDevice& device, const vlk::MemoryAllocator& allocator, const VkExtent2D& extent) :
    // VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT
    format_(VK_FORMAT_D32_SFLOAT_S8_UINT),
    image_("depth image", allocator, VK_IMAGE_TYPE_2D,
        VkExtent3D{ extent.width, extent.height ,1 }, format_, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VMA_MEMORY_USAGE_GPU_ONLY),
    image_view_(device, image_.Get(), format_, VK_IMAGE_ASPECT_DEPTH_BIT) {
    base::Log::Info("Renderer: depth image created");
}

const VkFormat& DepthImage::GetFormat() const {
    return format_;
}

const VkImageView& DepthImage::GetView() const {
    return image_view_.Get();
}

}; // renderer