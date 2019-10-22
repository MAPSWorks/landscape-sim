#pragma once
#include <string_view>
#include <memory>
#include "vlk/image.h"
#include "vlk/memory_allocator.h"

// Representation of image as 2D texture.
// Handles loading image from file, creating image object and copying data
// to the image.
namespace renderer {
class Texture2D {
public:
    // name - informative name of texture object.
    // file_name - file name this texture is loaded from.
    // allocator - memory allocator with which this texture object is allocated with.
    Texture2D(std::string_view name, std::string_view file_name, const vlk::MemoryAllocator& allocator);
    const VkImage& Get() const;
private:
    // Will hold image and it's memory.
    // Unique pointer for lazy-initialization.
    std::unique_ptr<const vlk::Image> image_;
};
}; //renderer