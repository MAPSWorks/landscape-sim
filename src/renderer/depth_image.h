#pragma once
#include "vlk/image.h"
#include "vlk/image_view.h"
#include "vlk/memory_allocator.h"

// Image to store pixel depth information.
// Depth image resource, view and format properties.
namespace renderer {
class DepthImage {
public:
    DepthImage(const VkDevice& device, const vlk::MemoryAllocator& allocator, const VkExtent2D& extent);
    const VkFormat& GetFormat() const;
    const VkImageView& GetView() const;
private:
    // Depth image format
    const VkFormat format_;
    const vlk::Image image_;
    const vlk::ImageView image_view_;
};
}; // renderer