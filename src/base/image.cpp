#include "image.h"
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "log.h"

namespace base {
Image::Image(const std::string& file_name) :
    // Force 4 channels
    forced_channel_count_(4){
    LoadImage(file_name);
}

Image::~Image() {
    stbi_image_free(image_);
}

const t::UC* Image::GetImage() const {
    return image_;
}

const t::Size32 Image::GetDimensions() const {
    return dimensions_;
}

// NOTE: for 8 bit channel no need to calculate by byte number 
t::U32 Image::GetSize() const {
    return dimensions_.width * dimensions_.height * forced_channel_count_;
}

void Image::LoadImage(const std::string& file_name) {
    int width, height, num_channels;
    // Force image to also have all 4 channels
    image_ = stbi_load(file_name.c_str(), &width, &height, &num_channels, STBI_rgb_alpha);
    if (!image_) {
        throw std::runtime_error("Base: failed to load image '" + file_name + "'");
    }
    dimensions_.width = static_cast<t::U32>(width);
    dimensions_.width = static_cast<t::U32>(height);
    Log::Info("Base: image (8bit) loaded '", file_name, "', channel num. (", num_channels, "), size (", dimensions_.width, "x",
        dimensions_.height, "). Forced to channel count (", forced_channel_count_, ")");
}

}; // base