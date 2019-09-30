#include "buffer.h"
#include <base/log.h>

namespace renderer::vlk {
Buffer::Buffer(const std::string name, const MemoryAllocator& allocator, BufferSize size, VkBufferUsageFlags usage,
    VmaMemoryUsage memory_usage) :
    name_(name),
    allocator_(allocator),
    buffer_(Create(name_, size, usage, memory_usage, allocation_)),
    size_(size) {
    base::Log::Info("Renderer: buffer '",name_,"' created");
    AllocationDebugPrint();
}

Buffer::~Buffer() {
    base::Log::Info("Renderer: buffer '", name_,"' destroying...");
    vmaDestroyBuffer(allocator_.Get(), buffer_, allocation_);
}

const VkBuffer& Buffer::Get() const {
    return buffer_;
}

const BufferSize Buffer::GetSize() const {
    return size_;
}

void Buffer::MapAndFill(BufferSize buffer_size, const void* vertex_data) const {
    void* data;
    ErrorCheck(vmaMapMemory(allocator_.Get(), allocation_, &data));
    memcpy(data, vertex_data, (size_t)buffer_size);
    vmaUnmapMemory(allocator_.Get(), allocation_);
} 

// NOTE: When info is retrieved, allocation is automatically 'touched'
// keep that in mind if there is difference between logged an non-logged runs
void Buffer::AllocationDebugPrint() const {
    if (base::Log::enabled) {
        VmaAllocationInfo info;
        vmaGetAllocationInfo(allocator_.Get(), allocation_, &info);
        base::Log::Info("Renderer: buffer '", name_, "' allocation info:");
        base::Log::Info("   memory type index - ", info.memoryType);
        base::Log::Info("   offset - ", info.offset);
        base::Log::Info("   size - ", info.size);
        const char* user_data = (const char*)info.pUserData;
        base::Log::Info("   user data - ", user_data);
    }
}

VkBuffer Buffer::Create(std::string name, BufferSize size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage, 
    VmaAllocation& allocation) {
    VkBufferCreateInfo buffer_info {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    // Buffer can be referenced only from queues of one family at a time
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // Amd allocation
    VmaAllocationCreateInfo alloc_info {};
    alloc_info.usage = memory_usage;
    // Only to store informative name of allocation for debugging
    alloc_info.flags = VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT;
    alloc_info.pUserData = const_cast<char*>(name.c_str());
    VkBuffer vertex_buffer;
    // - VkBuffer is created.
    // - VkDeviceMemory block is allocated if needed.
    // - An unused region of the memory block is bound to this buffer.
    ErrorCheck(vmaCreateBuffer(allocator_.Get(), &buffer_info, &alloc_info, &vertex_buffer, &allocation, nullptr));
    return vertex_buffer;
}
}; // renderer::vlk