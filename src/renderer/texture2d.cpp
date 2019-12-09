#include "texture2d.h"
#include <base/log.h>
#include "vlk/buffer.h"

namespace renderer {
Texture2D::Texture2D(std::string_view name, std::string_view file_name, const Renderer& renderer,
    t::U16 forced_channel_count, DataFormat data_format) {
    // Load file (no longer needed after data is copied to image memory)
    base::ImageFile texture_file(file_name, forced_channel_count);
    // Image format
    const VkFormat format = SelectFormat(texture_file.GetChannelCount(), texture_file.GetBitsPerChannel(), data_format);
    // Create image object
    image_ = std::make_unique<const vlk::Image>(name, renderer.GetMemoryAllocator(), VK_IMAGE_TYPE_2D, 
        VkExtent3D{ texture_file.GetDimensions().width , texture_file.GetDimensions().height, 1 }, format, 
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VMA_MEMORY_USAGE_GPU_ONLY);
    // Write data loaded from file to image memory
    CopyData(name, renderer, texture_file);
    // Create image view for texture
    image_view_ = std::make_unique<const vlk::ImageView>(renderer.GetContext().device.Get(), image_->Get(), format);
}

const VkImage& Texture2D::Get() const {
    return image_->Get();
}

const vlk::Image& Texture2D::GetImage() const {
    return *image_;
}

const VkImageView& Texture2D::GetImageView() const {
    return image_view_->Get();
}

// NOTE: add formats as needed
VkFormat Texture2D::SelectFormat(t::U16 channel_count, t::U16 bits_per_channel, DataFormat data_format) const {
    VkFormat format = VK_FORMAT_UNDEFINED;
    if (bits_per_channel == 16) {
        switch (data_format)
        {
            case renderer::Texture2D::DataFormat::lUNorm:
                if (channel_count == 1) {
                    format = VK_FORMAT_R16_UNORM;
                }
                else if (channel_count == 4) {
                    format = VK_FORMAT_R16G16B16A16_UNORM;
                }
                break;
            case renderer::Texture2D::DataFormat::kUInt:
                if (channel_count == 1) {
                    format = VK_FORMAT_R16_UINT;
                }
                else if (channel_count == 4) {
                    format = VK_FORMAT_R16G16B16A16_UINT;
                }
                break;
            default:
                break;
        }
    }
    else if (bits_per_channel == 8) {
        switch (data_format)
        {
            case renderer::Texture2D::DataFormat::lUNorm:
                if (channel_count == 1) {
                    format = VK_FORMAT_R8_UNORM;
                }
                else if (channel_count == 4) {
                    format = VK_FORMAT_R8G8B8A8_UNORM;
                }
                break;
            case renderer::Texture2D::DataFormat::kUInt:
                if (channel_count == 1) {
                    format = VK_FORMAT_R8_UINT;
                }
                else if (channel_count == 4) {
                    format = VK_FORMAT_R8G8B8A8_UINT;
                }
                break;
            default:
                break;
        }
    }
    
    if (format == VK_FORMAT_UNDEFINED) {
        throw std::runtime_error("Renderer: unsupported texture format");
    }
    return format;
}

void Texture2D::TransitionImageLayout(const OneTimeCommands& one_time_commands, VkImageLayout old_layout,
    VkImageLayout new_layout) const {
    auto barrier = image_->GetMemoryBarrier(old_layout, new_layout);
    // MOdifie barrier and set pipeline stages before and after barrier
    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags dest_stage;
    // When transfer is from 'undefined' to 'transfer'
    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        // Don't wait on anything
        barrier.srcAccessMask = 0;
        // At transfer
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        // Earliest stage
        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        // Occurs in transfer pseudo pipeline stage
        dest_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    // When transfer is from 'transfer' to 'read in shader'
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        // Tansfer has to be done
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        // Will be read in shader
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        // OPTI: this image is accessible in all pipeline stages.
        // It could be more optimal only the stages this texture is going to be used in
        dest_stage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    }
    else {
        throw std::runtime_error("Renderer: unsupported layout transition");
    }
    one_time_commands.PipelineImageMemoryBarrier(source_stage, dest_stage, 0, 1, &barrier);
}

// * Create staging buffer to store image data
// * Transition image from undefined to transfer
// * Copy data from staging buffer to image
// * Transition image from transfer layout to read-in-shader layout
void Texture2D::CopyData(std::string_view name, const Renderer& renderer, const base::ImageFile& texture_file) const {
    const auto texture_dims = texture_file.GetDimensions();
    const auto texture_size = texture_file.GetSize();
    // Create staging buffer and copy data from file
    const vlk::Buffer staging_buffer(static_cast<std::string>(name) + " staging", renderer.GetMemoryAllocator(),
        texture_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
    staging_buffer.MapAndFill(texture_size, texture_file.GetImage());
    base::Log::Info("Renderer: texture staging buffer '", name, "' mapped and filled with data, size - ", texture_size);
    // Change layout for recording
    TransitionImageLayout(renderer.GetOneTimeCommands(), VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    // Copy from staging buffer to image
    renderer.GetOneTimeCommands().CopyBufferToImage2D(staging_buffer.Get(), image_->Get(), texture_dims);
    // Change layout for sampling in shader
    TransitionImageLayout(renderer.GetOneTimeCommands(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}
}; //renderer