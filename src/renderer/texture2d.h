#pragma once
#include <string_view>
#include <memory>
#include "vlk/image.h"
#include "vlk/image_view.h"
#include "renderer.h"

// Representation of image as 2D texture.
// Handles loading image from file, creating image object and copying data
// to the image.
namespace renderer {
class Texture2D {
public:
    // Texture data format
    enum class DataFormat {
        // Texture values goe from 0.0 - 1.0 (default)
        lUNorm,
        // Texture values are not normalized and go from 0 to integer number limited by bit depth
        kUInt,
    };
    // name - informative name of texture object.
    // file_name - file name this texture is loaded from.
    // renderer - used to retrieve memory allocator with which this texture object is allocated with.
    //            and one time command helper to execute commands.
    // forced_channel_count - how many channels to force to when loading image from file.
    //                        0 - the same number as in file, 1 to 4 otherwise to specify desired count.
    // data_format - data format of the texture. Afects returned value from sampling texture in shaders.
    Texture2D(std::string_view name, std::string_view file_name, const Renderer& renderer, 
        t::U16 forced_channel_count = 0, DataFormat data_format = DataFormat::lUNorm);
    const VkImage& Get() const;
    const VkImageView& GetImageView() const;
private:
    // Select appropriate texture format for storing in memory based on given parameter
    // combination
    VkFormat SelectFormat(t::U16 channel_count, t::U16 bits_per_channel, DataFormat data_format) const;
    // Change image layout in memory
    void TransitionImageLayout(const OneTimeCommands& one_time_commands, VkImageLayout old_layout,
        VkImageLayout new_layout) const;
    // Will hold image and it's memory.
    // Unique pointer for lazy-initialization.
    std::unique_ptr<const vlk::Image> image_;
    // Texture image will be accessed through image view
    std::unique_ptr <const vlk::ImageView> image_view_;
};
}; //renderer