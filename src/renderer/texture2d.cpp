#include "texture2d.h"
#include <base/log.h>
#include <base/image_file.h>
#include "vlk/buffer.h"

namespace renderer {
Texture2D::Texture2D(std::string_view name, std::string_view file_name, const Renderer& renderer)
{
    // Load file
    base::ImageFile texture_file(file_name);
    const auto texture_dims = texture_file.GetDimensions();
    const auto texture_size = texture_file.GetSize();
    // Create image object
    image_ = std::make_unique<const vlk::Image>(name, renderer.GetMemoryAllocator(), VK_IMAGE_TYPE_2D, 
        VkExtent3D{ texture_dims.width , texture_dims.height, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY);
    // Create staging buffer and copy data from file
    const vlk::Buffer staging_buffer(static_cast<std::string>(name) + " staging", renderer.GetMemoryAllocator(), 
        texture_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
    staging_buffer.MapAndFill(texture_size, texture_file.GetImage());
    base::Log::Info("Renderer: texture staging buffer '", name, "' mapped and filled with data, size - ", texture_size);
    // Change layout for recording
    TransitionImageLayout(renderer.GetOneTimeCommands(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
}

const VkImage& Texture2D::Get() const {
    return image_->Get();
}

void Texture2D::TransitionImageLayout(const OneTimeCommands& one_time_commands, VkFormat format, VkImageLayout old_layout,
    VkImageLayout new_layout) const {
    const auto barrier = image_->GetMemoryBarrier(format, old_layout, new_layout);
    one_time_commands.PipelineImageMemoryBarrier(0,0,0,1, &barrier);
}
}; //renderer