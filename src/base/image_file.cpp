#include "image_file.h"
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "log.h"

namespace base {
ImageFile::ImageFile(std::string_view file_name, t::I16 channels) :
    channel_count_(channels){
    assert(channel_count_ >= 0 && channel_count_ <= 4);
    LoadImage(file_name.data());
}

ImageFile::~ImageFile() {
    if (Is16bit()) {
        stbi_image_free(image_16_);
    } else {
        stbi_image_free(image_8_);
    }
}

const t::UC* ImageFile::GetImage8() const {
    return image_8_;
}

const t::US* ImageFile::GetImage16() const {
    return image_16_;
}

const t::Size32& ImageFile::GetDimensions() const {
    return dimensions_;
}

const bool ImageFile::Is16bit() const {
    return (bytes_per_channel_ == 2);
}

// NOTE: for 8 bit channel no need to calculate by byte number 
t::U32 ImageFile::GetSize() const {
    return dimensions_.width * dimensions_.height * channel_count_ * bytes_per_channel_;
}

t::US ImageFile::GetGray16At(t::U32 x, t::U32 y) const {
    if (x < 0 || x >= dimensions_.width || y < 0 || y >= dimensions_.height) {
        throw std::runtime_error("Base: image access index out of bounds");
    }
    return *(image_16_ + (y * dimensions_.width + x));
}

void ImageFile::LoadImage(const std::string& file_name) {
    // num_channels is original number of channel in image
    // channel_count_ is forced channel count, unless it is 0, then originals will be used
    int width = 0, height = 0, num_channels = 0;
    // How many bit-per-channel in image
    // 16-bit
    if (stbi_is_16_bit(file_name.c_str())) {
        bytes_per_channel_ = 2;
        image_16_ = stbi_load_16(file_name.c_str(), &width, &height, &num_channels, channel_count_);
        if (!image_16_) {
            throw std::runtime_error("Base: failed to load image (16 bits)'" + file_name + "'");
        }
    }
    // 8-bit
    else {
        bytes_per_channel_ = 1;
        image_8_ = stbi_load(file_name.c_str(), &width, &height, &num_channels, channel_count_);
        if (!image_8_) {
            throw std::runtime_error("Base: failed to load image (8 bits)'" + file_name + "'");
        }
    }
    if (channel_count_ == 0) {
        channel_count_ = num_channels;
    }
    dimensions_.width = static_cast<t::U32>(width);
    dimensions_.height = static_cast<t::U32>(height);
    Log::Info("Base: image (",(bytes_per_channel_*8),"bit) loaded '", file_name, "', size (", dimensions_.width, "x",
        dimensions_.height, "), channel count: original (", channel_count_, "), forced (", num_channels, ")");
}

}; // base