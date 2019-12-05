#include "image.h"
#include <base/log.h>

namespace renderer::vlk {
Image::Image(std::string_view name, const MemoryAllocator& allocator, VkImageType type, VkExtent3D extent,
    VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memory_usage) :
    name_(name),
    allocator_(allocator),
    image_(Create(name_, type, extent, format, tiling, usage, memory_usage, allocation_)) {
    base::Log::Info("Renderer: image '", name_, "' created");
    AllocationDebugPrint();
}

Image::~Image() {
    base::Log::Info("Renderer: image '", name_, "' destroying...");
    vmaDestroyImage(allocator_.Get(), image_, allocation_);
}

const VkImage& Image::Get() const {
    return image_;
}

// NOTE: When info is retrieved, allocation is automatically 'touched'
// keep that in mind if there is difference between logged an non-logged runs
void Image::AllocationDebugPrint() const {
    if (base::Log::enabled) {
        VmaAllocationInfo info;
        vmaGetAllocationInfo(allocator_.Get(), allocation_, &info);
        base::Log::Info("Renderer: image '", name_, "' allocation info:");
        base::Log::Info("   memory type index - ", info.memoryType);
        base::Log::Info("   offset - ", info.offset);
        base::Log::Info("   size - ", info.size);
        const char* user_data = (const char*)info.pUserData;
        base::Log::Info("   user data - ", user_data);
    }
}

VkImageMemoryBarrier Image::GetMemoryBarrier(VkImageLayout old_layout, VkImageLayout new_layout) const {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image_;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    // NOTE: access masks modified elsewhere right before usage, because they depend on usage
    barrier.srcAccessMask = 0; 
    barrier.dstAccessMask = 0;
    return barrier;
}

VkImage Image::Create(std::string name, VkImageType type, VkExtent3D extent, VkFormat format, VkImageTiling tiling,
    VkImageUsageFlags usage, VmaMemoryUsage memory_usage, VmaAllocation& allocation) {
    VkImageCreateInfo image_info{};
    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.imageType = type;
    image_info.extent = extent;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    // Buffer can be referenced only from queues of one family at a time
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // Amd allocation
    VmaAllocationCreateInfo alloc_info{};
    alloc_info.usage = memory_usage;
    // Only to store informative name of allocation for debugging
    alloc_info.flags = VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT;
    alloc_info.pUserData = const_cast<char*>(name.c_str());
    VkImage image;
    // - VkImage is created.
    // - VkDeviceMemory block is allocated if needed.
    // - An unused region of the memory block is bound to this buffer.
    ErrorCheck(vmaCreateImage(allocator_.Get(), &image_info, &alloc_info, &image, &allocation, nullptr));
    return image;
}
}; // renderer::vlk