#include "texture2d.h"
#include <base/log.h>
#include <base/image_file.h>

namespace renderer {
Texture2D::Texture2D(std::string_view name, std::string_view file_name, const vlk::MemoryAllocator& allocator)
{
    // Load file
    base::ImageFile texture_file(file_name);
    const auto texture_dims = texture_file.GetDimensions();
    // Create image object
    image_ = std::make_unique<const vlk::Image>(name, allocator, VK_IMAGE_TYPE_2D, 
        VkExtent3D{ texture_dims.width , texture_dims.height, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY);
    // Create staging buffer and copy data from file

}

const VkImage& Texture2D::Get() const {
    return image_->Get();
}
}; //renderer