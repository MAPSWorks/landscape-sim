#pragma once
#include <string>
#include "types.h"
#include "memory_allocator.h"
#include "vulkan_shared.h"

// Abstraction of buffer in general.
// Creates buffer and allocates memory.
// Further used to inmplement vertex, index buffers, etc  
namespace renderer::vlk {
class Buffer {
public:
    // name - informative name
    // allocator - global allocator this buffer will be allocated with
    // size - size of data to be fitted into buffer
    // usage - what this buffer will be used for
    // memory_usage - type of a memory used (GPU local, CPU local etc)
    Buffer(std::string_view name, const MemoryAllocator& allocator, BufferSize size, VkBufferUsageFlags usage, 
        VmaMemoryUsage memory_usage);
    ~Buffer();
    Buffer(Buffer const&) = delete;
    Buffer operator=(Buffer const&) = delete;
    const VkBuffer& Get() const;
    const BufferSize GetSize() const;
    // Works for mappable memory type
    void MapAndFill(BufferSize buffer_size, const void* vertex_data) const;
    // Retrieve information about current allocation
    void AllocationDebugPrint() const;
private:
    // Allocation is created in this f-tion and returned through reference
    VkBuffer Create(std::string name, BufferSize size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage, 
        VmaAllocation& allocation);
    // Informative name of a buffer, mainly for debugging purposes
    const std::string name_;
    // Reference to resource this renderable is created with.
    // This global renderable manages all allocations, each separate allocation is stored in
    // alocator_ member
    const MemoryAllocator& allocator_;
    // Object that represents memory assigned to this buffer.It can be queried 
    // for parameters like Vulkan memory handle and offset.
    VmaAllocation allocation_;
    const VkBuffer buffer_ = VK_NULL_HANDLE;
    // Buffer size upon creation
    // This value is recieved and stored in constructor.
    // NOTE: in some cases actual size that is filled might be different than the one created
    const BufferSize size_;
};
}; // renderer::vlk