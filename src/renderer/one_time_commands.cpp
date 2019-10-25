#include "one_time_commands.h"
#include <base/log.h>
#include "vlk/command_buffer.h"

namespace renderer {
OneTimeCommands::OneTimeCommands(const vlk::Device& device) :
    device_(device),
    // Imply that graphics family suports all other needed commands
    command_pool_(device_.Get(), device_.GetQueue().GetFamilyIndices().graphics.value(), false, true) {
    base::Log::Info("Renderer: one-time submit commands helper created");
}

// Source buffer must be host visible and transfer source, destination buffer must be transfer destination
void OneTimeCommands::CopyBuffer(const VkBuffer& src_buffer, const VkBuffer& dst_buffer, vlk::BufferSize size) const {
    vlk::CommandBuffer command_buffer(command_pool_);
    command_buffer.Begin(vlk::CommandBuffer::Usage::kOneTimeSubmit);
    command_buffer.CopyBuffer(src_buffer, dst_buffer, size);
    command_buffer.End();
    // Submit to graphics queue because graphics queue implicitly supports transfer
    device_.GetQueue().Submit(device_.GetQueue().GetGraphics(), command_buffer.Get());
    // Since we dont use fence
    vlk::ErrorCheck(vkQueueWaitIdle(device_.GetQueue().GetGraphics()));
    // Freeing is not in command buffers destructor because it is not mandatory.
    // When pool is destroyed so are command buffers.
    command_buffer.Free(device_.Get());
    base::Log::Info("Renderer: buffer copy performed. Size - ", size);
}

void OneTimeCommands::PipelineImageMemoryBarrier(VkPipelineStageFlags src_stagemask, VkPipelineStageFlags dst_stagemask,
    VkDependencyFlags dependancy_flags, t::U32 image_memory_barrier_count, const VkImageMemoryBarrier* image_memory_barrier) const {
    vlk::CommandBuffer command_buffer(command_pool_);
    command_buffer.Begin(vlk::CommandBuffer::Usage::kOneTimeSubmit);
    command_buffer.PipelineImageMemoryBarrier(src_stagemask, dst_stagemask, dependancy_flags, 
        image_memory_barrier_count, image_memory_barrier);
    command_buffer.End();
    // Submit to graphics queue because graphics queue implicitly supports transfer
    device_.GetQueue().Submit(device_.GetQueue().GetGraphics(), command_buffer.Get());
    // Since we dont use fence
    vlk::ErrorCheck(vkQueueWaitIdle(device_.GetQueue().GetGraphics()));
    // Freeing is not in command buffers destructor because it is not mandatory.
    // When pool is destroyed so are command buffers.
    command_buffer.Free(device_.Get());
    base::Log::Info("Renderer: image memory barrier recording performed");
}

// NOTE: Image layout is assumed to be in VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
void OneTimeCommands::CopyBufferToImage2D(const VkBuffer& buffer, const VkImage& image, const t::Size32& dimensions) const {
    vlk::CommandBuffer command_buffer(command_pool_);
    command_buffer.Begin(vlk::CommandBuffer::Usage::kOneTimeSubmit);
    command_buffer.CopyBufferToImage2D(buffer, image, dimensions, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    command_buffer.End();
    // Submit to graphics queue because graphics queue implicitly supports transfer
    device_.GetQueue().Submit(device_.GetQueue().GetGraphics(), command_buffer.Get());
    // Since we dont use fence
    vlk::ErrorCheck(vkQueueWaitIdle(device_.GetQueue().GetGraphics()));
    // Freeing is not in command buffers destructor because it is not mandatory.
    // When pool is destroyed so are command buffers.
    command_buffer.Free(device_.Get());
    base::Log::Info("Renderer: buffer to image copy performed. Dims - (" ,dimensions.width,", ", dimensions.width, ")");
}
}; //renderer