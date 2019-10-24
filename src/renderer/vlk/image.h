#pragma once
#include <string>
#include "types.h"
#include "memory_allocator.h"
#include "vulkan_shared.h"

// Image abstraction
// Creates image object and allocates memory and binds it to the image.
// Use VMA to assist memory allocation and binding
namespace renderer::vlk {
class Image {
public:
    Image(std::string_view name, const MemoryAllocator& allocator, VkImageType type, VkExtent3D extent,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memory_usage);
    ~Image();
    Image(Image const&) = delete;
    Image operator=(Image const&) = delete;
    const VkImage& Get() const;
    // Retrieve information about current allocation
    void AllocationDebugPrint() const;
    // Change image layout
    void TransitionLayout(VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout) const;
private:
    // Allocation is created in this f-tion and returned through reference
    VkImage Create(std::string name, VkImageType type, VkExtent3D extent, VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage, VmaMemoryUsage memory_usage, VmaAllocation& allocation);
    // Informative name of an image, mainly for debugging purposes
    const std::string name_;
    // Reference to resource this renderable is created with.
    // This global renderable manages all allocations, each separate allocation is stored in
    // allocator_ member
    const MemoryAllocator& allocator_;
    // Object that represents memory assigned to this image.It can be queried 
    // for parameters like Vulkan memory handle and offset.
    VmaAllocation allocation_;
    const VkImage image_ = VK_NULL_HANDLE;
};
}; // renderer::vlk