#pragma once
#include <string>
#include "buffer.h"
#include "memory_allocator.h"
#include "types.h"

// Buffer to store vertex data on GPU side.
// This buffer is for static data - set once at start-up and used often
namespace renderer::vlk {
class VertexBuffer {
public:
    VertexBuffer(const std::string name, const MemoryAllocator& allocator, BufferSize buffer_size, 
        const void* buffer_data);
    const VkBuffer& Get() const;
private:
    // Fill given buffer with data
    void FillBuffer(const std::string name, const MemoryAllocator& allocator, BufferSize buffer_size,
        const void* buffer_data, const VkBuffer& buffer) const;
    const Buffer buffer_;
};
}; // renderer vlk