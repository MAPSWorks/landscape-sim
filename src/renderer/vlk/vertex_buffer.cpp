#include "vertex_buffer.h"
#include <stdexcept>
#include <base/log.h>

namespace renderer::vlk {
VertexBuffer::VertexBuffer(const Device& device, uint64_t buffer_size) :
    device_(device),
    vertex_buffer_(Create(buffer_size)) {
    AllocateMemory();
    base::Log::Info("Renderer: vertex buffer created");
}

VertexBuffer::~VertexBuffer() {
    base::Log::Info("Renderer: vertex buffer destroying...");
    vkDestroyBuffer(device_.Get(), vertex_buffer_, nullptr);
    vkFreeMemory(device_.Get(), buffer_memory_, nullptr);
}

const VkBuffer& VertexBuffer::Get() const {
    return vertex_buffer_;
}

// Sending data from cpu to gpu doesnt neceserily happen at this moment
// but in our case is guaranteed before submiting to queue
void VertexBuffer::Fill(const void* vertex_data) const {
    void* data;
    ErrorCheck(vkMapMemory(device_.Get(), buffer_memory_, 0, actual_buffer_size_, 0, &data));
    memcpy(data, vertex_data, (size_t)actual_buffer_size_);
    vkUnmapMemory(device_.Get(), buffer_memory_);
}

VkBuffer VertexBuffer::Create(uint64_t buffer_size) const {
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = buffer_size;
    buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    // Buffer can be referenced only from queues of one family at a time
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkBuffer vertex_buffer;
    ErrorCheck(vkCreateBuffer(device_.Get(), &buffer_info, nullptr, &vertex_buffer));
    return vertex_buffer;
}

void VertexBuffer::AllocateMemory() {
    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(device_.Get(), vertex_buffer_, &mem_requirements);
    actual_buffer_size_ = mem_requirements.size;
    VkMemoryAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    // We may need to allocate more memory than size of a buffer
    alloc_info.allocationSize = actual_buffer_size_;
    // Find a memory type that corresponds to buffer requirements
    alloc_info.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, 
        // Data to be oploaded directly from CPU
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
        // Upload will actually take place in time before usage
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    ErrorCheck(vkAllocateMemory(device_.Get(), &alloc_info, nullptr, &buffer_memory_));
    ErrorCheck(vkBindBufferMemory(device_.Get(), vertex_buffer_, buffer_memory_, 0));
    base::Log::Info("Renderer: vertex buffer memory allocated. Size - ", actual_buffer_size_);
}

// Get memory GPU memory type that satisfies requirements from params
uint32_t VertexBuffer::FindMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties) const {
    const auto memory_properties = device_.GetGPUMemoryProperties();
    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
        // Check if coresponding bit is set to 1
        if ((type_filter & (1 << i)) && 
            // Comparison is because we are interested not only in non-zero result
            // but the one that matches desired properties bit field
            (memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("Renderer: failed to find vertex buffer memory type");
}


}; // renderer vlk