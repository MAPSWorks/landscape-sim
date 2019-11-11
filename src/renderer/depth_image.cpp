#include "depth_image.h"

namespace renderer {
DepthImage::DepthImage(const VkDevice& device, const vlk::MemoryAllocator& allocator, const VkExtent2D& extent) :
    // Specifies a two - component, 32 - bit packed format that has 8 unsigned integer bits in the stencil component, 
    // and 24 unsigned normalized bits in the depth component
    format_(VK_FORMAT_D24_UNORM_S8_UINT),
    image_("depth image", allocator, VK_IMAGE_TYPE_2D,
        VkExtent3D{ extent.width, extent.height ,1 }, format_, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VMA_MEMORY_USAGE_GPU_ONLY),
    image_view_(device, image_.Get(), format_, VK_IMAGE_ASPECT_DEPTH_BIT)
{
}

const VkFormat& DepthImage::GetFormat() const {
    return format_;
}

}; // renderer