#include "index_buffer.h"
#include <base/log.h>

namespace renderer {
IndexBuffer::IndexBuffer(const std::string& name, const Renderer& renderer, vlk::BufferSize buffer_size,
    const void* buffer_data) :
    buffer_(name, renderer.GetMemoryAllocator(), buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY) {
    FillBuffer(name, renderer, buffer_size, buffer_data, buffer_.Get());
    base::Log::Info("Renderer: index buffer '", name, "' initialized");
}

const VkBuffer& IndexBuffer::Get() const {
    return buffer_.Get();
}

void IndexBuffer::FillBuffer(const std::string& name, const Renderer& renderer, vlk::BufferSize buffer_size,
    const void* buffer_data, const VkBuffer& buffer) const {
    // Data write to buffer is performed with staging buffer and copy
    // Staging buffer is host visible, mappable memory buffer.
    // that is used to store original data and then this data gets copied to
    // GPU local memory buffer which is faster.
    const vlk::Buffer staging_buffer(name + " staging", renderer.GetMemoryAllocator(), buffer_size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
    staging_buffer.MapAndFill(buffer_size, buffer_data);
    base::Log::Info("Renderer: index staging buffer '", name, "' mapped and filled with data, size - ", buffer_size);
    renderer.GetOneTimeCommands().CopyBuffer(staging_buffer.Get(), buffer, buffer_size);
}
}; // renderer