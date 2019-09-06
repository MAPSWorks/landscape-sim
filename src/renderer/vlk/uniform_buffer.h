#pragma once
#include "buffer.h"
#include "memory_allocator.h"

// Uniform buffer stores global shader variables
namespace renderer::vlk {
class UniformBuffer {
public:
    UniformBuffer(const std::string name, const MemoryAllocator& allocator, BufferSize buffer_size);
    const VkBuffer& Get() const;
    // When contents of a buffer change in real time use this
    void Update(const void* buffer_data) const;
private:
    const Buffer buffer_;
};
}; // renderer vlk