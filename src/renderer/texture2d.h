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
    // name - informative name of texture object.
    // file_name - file name this texture is loaded from.
    // renderer - used to retrieve memory allocator with which this texture object is allocated with.
    //            and one time command helper to execute commands.
    Texture2D(std::string_view name, std::string_view file_name, const Renderer& renderer);
    const VkImage& Get() const;
    const VkImageView& GetImageView() const;
private:
    // Change image layout in memory
    void TransitionImageLayout(const OneTimeCommands& one_time_commands, VkFormat format, VkImageLayout old_layout,
        VkImageLayout new_layout) const;
    // Will hold image and it's memory.
    // Unique pointer for lazy-initialization.
    std::unique_ptr<const vlk::Image> image_;
    // Texture image will be accessed through image view
    std::unique_ptr <const vlk::ImageView> image_view_;
};
}; //renderer