#include "vertex_buffer.h"
#include <base/log.h>

namespace renderer::vlk {
VertexBuffer::VertexBuffer(const std::string name, const MemoryAllocator& allocator, BufferSize buffer_size, 
    const void* buffer_data) :
    buffer_(name, allocator, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VMA_MEMORY_USAGE_GPU_ONLY) {
    FillBuffer(name, allocator, buffer_size, buffer_data, buffer_.Get());
    base::Log::Info("Renderer: vertex buffer '", name, "' initialized");
}

const VkBuffer& VertexBuffer::Get() const {
    return buffer_.Get();
}

void VertexBuffer::FillBuffer(const std::string name, const MemoryAllocator& allocator, BufferSize buffer_size,
    const void* buffer_data, const VkBuffer& buffer) const {
    // Data write to buffer is performed with staging buffer and copy
    // Staging buffer is host visible, mappable memory buffer.
    // that is used to store original data and then this data gets copied to
    // GPU local memory buffer which is faster.
    const Buffer staging_buffer_(name + " staging", allocator, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
    staging_buffer_.MapAndFill(buffer_size, buffer_data);
    allocator.CopyBuffer(staging_buffer_.Get(), buffer, buffer_size);
}
}; // renderer vlk