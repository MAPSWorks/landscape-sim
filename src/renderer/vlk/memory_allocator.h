#pragma once
#undef APIENTRY // to remove the warning (potential source of problems?)
#include <allocator/vk_mem_alloc.h>
#include "vulkan_shared.h"
#include "device.h"

// Wrapper around AMD vulkan memory allocatr application wide renderable that manages
// buffer memory allocations.
// Only single instance of this class should exist
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
private:
    VmaAllocator Create(const Device& device) const;
    // AMD memory allocator
    const VmaAllocator allocator_;
};
}; // renderer::vlk