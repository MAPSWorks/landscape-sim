#include "image_file.h"
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "log.h"

namespace base {
ImageFile::ImageFile(std::string_view file_name) :
    // Force 4 channels (r,g,b,a)
    forced_channel_count_(4){
    LoadImage(file_name.data());
}

ImageFile::~ImageFile() {
    stbi_image_free(image_);
}

const t::UC* ImageFile::GetImage() const {
    return image_;
}

const t::Size32& ImageFile::GetDimensions() const {
    return dimensions_;
}

// NOTE: for 8 bit channel no need to calculate by byte number 
t::U32 ImageFile::GetSize() const {
    return dimensions_.width * dimensions_.height * forced_channel_count_;
}

void ImageFile::LoadImage(const std::string& file_name) {
    int width, height, num_channels;
    // Force image to also have all 4 channels
    image_ = stbi_load(file_name.c_str(), &width, &height, &num_channels, STBI_rgb_alpha);
    if (!image_) {
        throw std::runtime_error("Base: failed to load image '" + file_name + "'");
    }
    dimensions_.width = static_cast<t::U32>(width);
    dimensions_.height = static_cast<t::U32>(height);
    Log::Info("Base: image (8bit) loaded '", file_name, "', channel num. (", num_channels, "), size (", dimensions_.width, "x",
        dimensions_.height, "). Forced to channel count (", forced_channel_count_, ")");
}

}; // base