#pragma once
#include <string>
#include "types.h"

// Load in CPU memeory and store 8-bit (per channel) image.
// Channels are forced rgba.
namespace base {
class Image {
public:
    Image(const std::string& file_name);
    ~Image();
    // non-copyable
    Image(Image const&) = delete;
    Image operator=(Image const&) = delete;
    // Get pointer to image data
    const t::UC* GetImage() const;
    const t::Size32 GetDimensions() const;
    // Image size in bytes (calculated on the fly from dimension)
    t::U32 GetSize() const;
private:
    void LoadImage(const std::string& file_name);
    // Number of forced channels that image will have
    // regardless of actual data in image file.
    const t::I16 forced_channel_count_;
    // Pointer to image data
    t::UC* image_ = nullptr;
    // Width and height in pixels
    t::Size32 dimensions_;
};
}; // base