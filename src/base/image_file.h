#pragma once
#include <string>
#include "types.h"

// Load in CPU memeory and store 8-bit or 16-bit (per channel) image from given file.
// Channels count is either original or forced
namespace base {
class ImageFile {
public:
    // file_name - name of the image file to load
    // channels - number of channels in image to force to (0 - means use original channel count)
    ImageFile(std::string_view file_name, t::I16 channels = 0);
    ~ImageFile();
    // non-copyable
    ImageFile(ImageFile const&) = delete;
    ImageFile operator=(ImageFile const&) = delete;
    // Get pointer to image data by bits
    const t::UC* GetImage8() const;
    const t::US* GetImage16() const;
    const t::Size32& GetDimensions() const;
    // Weather image is of 2 bytes per channel
    const bool Is16bit() const;
    // Image size in bytes (calculated on the fly from dimension)
    t::U32 GetSize() const;
    // Get value of 1-channel 16-bit texture at given coordinate, starting from top-left
    t::US GetGray16At(t::U32 x, t::U32 y) const;
private:
    void LoadImage(const std::string& file_name);
    // Stores how many bytes per channel are used
    t::I16 bytes_per_channel_ = 0;
    // Number of image channels. Either forced or original
    t::I16 channel_count_;
    // Pointer to image data of 8 - bits
    t::UC* image_8_ = nullptr;
    // Pointer to image data of 16 - bits
    t::US* image_16_ = nullptr;
    // Width and height in pixels
    t::Size32 dimensions_;
};
}; // base