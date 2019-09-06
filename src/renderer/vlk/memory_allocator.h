#pragma once
#undef APIENTRY // to remove the warning (potential source of problems?)
#include <allocator/vk_mem_alloc.h>
#include "vulkan_shared.h"
#include "device.h"
#include "command_pool.h"

// Wrapper around AMD vulkan memory allocatr application wide renderable that manages
// buffer memory allocations.
// Only single instance of this class should exist
// Also provide several helpers in buffer operations. These helpers hould probably be elshere.
namespace renderer::vlk {
class MemoryAllocator {
public:
    MemoryAllocator(const Device& device);
    ~MemoryAllocator();
    MemoryAllocator(MemoryAllocator const&) = delete;
    MemoryAllocator operator=(MemoryAllocator const&) = delete;
    const VmaAllocator& Get() const;
    // Retrieve and print statistics of this allocator
    void DebugPrint() const;
    // Helper
    // Copies contents of one buffer to other
    void CopyBuffer(const VkBuffer& src_buffer, const VkBuffer& dst_buffer, BufferSize size) const;
private:
    VmaAllocator Create(const Device& device) const;
    // Reference to resource this renderable is created with
    const Device& device_;
    // AMD memory allocator
    const VmaAllocator allocator_;
    // Helper
    // Command pool for short-lived command buffers for memory transfers from buffer to buffer.
    // Driver should be able to optimaze if we use separate pool for these transient operation.
    const CommandPool transfer_command_pool_;
};
}; // renderer::vlk