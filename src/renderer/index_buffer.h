#pragma once
#include <string>
#include "vlk/buffer.h"
#include "renderer.h"

// Buffer to store index data on GPU side.
// This buffer is for static data - set once at start-up and used often
namespace renderer {
class IndexBuffer {
public:
    IndexBuffer(const std::string& name, const Renderer& renderer, vlk::BufferSize buffer_size,
        const void* buffer_data);
    const VkBuffer& Get() const;
private:
    // Fill given buffer with data
    void FillBuffer(const std::string& name, const Renderer& renderer, vlk::BufferSize buffer_size,
        const void* buffer_data, const VkBuffer& buffer) const;
    const vlk::Buffer buffer_;
};
}; // renderer