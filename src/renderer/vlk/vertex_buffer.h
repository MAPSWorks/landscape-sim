#pragma once
#include "vulkan_shared.h"
#include "device.h"

namespace renderer::vlk {
class VertexBuffer {
public:
    VertexBuffer(const Device& device, uint64_t buffer_size);
    ~VertexBuffer();
    VertexBuffer(VertexBuffer const&) = delete;
    VertexBuffer operator=(VertexBuffer const&) = delete;
    const VkBuffer& Get() const;
    // Fill buffer with vertices
    void Fill(const void* vertex_data) const;
private:
    VkBuffer Create(uint64_t buffer_size) const;
    void AllocateMemory();
    uint32_t FindMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const;
    // Reference to resource this object is created with
    const Device& device_;
    const VkBuffer vertex_buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory buffer_memory_ = VK_NULL_HANDLE;
    // Required size from vertex buffer and actual required size may differ
    uint64_t actual_buffer_size_ = 0;
};
}; // renderer vlk