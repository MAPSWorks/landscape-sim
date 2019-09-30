#include "uniform_buffer.h"
#include <base/log.h>

namespace renderer::vlk {
UniformBuffer::UniformBuffer(const std::string name, const MemoryAllocator& allocator, BufferSize buffer_size):
    // Uniform buffer updates happen frequently therefore staging buffers are not recomended.
                                                                            // VMA_MEMORY_USAGE_CPU_TO_GPU,
    buffer_(name, allocator, buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_ONLY) {
    base::Log::Info("Renderer: dynamic uniform buffer '", name, "' initialized, but no data set");
}

const VkBuffer& UniformBuffer::Get() const {
    return buffer_.Get();
}

void UniformBuffer::Update(const void* buffer_data) const {
    buffer_.MapAndFill(buffer_.GetSize(), buffer_data);
}
}; // renderer vlk