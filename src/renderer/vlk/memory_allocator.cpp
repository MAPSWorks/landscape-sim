#include "memory_allocator.h"
#include <base/log.h>
//#include "command_buffer.h"

namespace renderer::vlk {
MemoryAllocator::MemoryAllocator(const Device& device):
    allocator_(Create(device)){
    base::Log::Info("Renderer: GPU memory allocator created");
}

MemoryAllocator::~MemoryAllocator() {
    base::Log::Info("Renderer: GPU memory allocator destroying...");
    vmaDestroyAllocator(allocator_);
}

const VmaAllocator& MemoryAllocator::Get() const {
    return allocator_;
}

// Retrieve and print statistics of this allocator
void MemoryAllocator::DebugPrint() const {
    if (base::Log::enabled) {
        VmaStats stats;
        vmaCalculateStats(allocator_, &stats);
        base::Log::Info("Renderer: GPU memory allocator statistics:");
        base::Log::Info("   block count - ", stats.total.blockCount);
        base::Log::Info("   allocation count - ", stats.total.allocationCount);
        base::Log::Info("   used bytes - ", stats.total.usedBytes);
        base::Log::Info("   unused bytes - ", stats.total.unusedBytes);
        base::Log::Info("   unused range count - ", stats.total.unusedRangeCount);
    }
}

VmaAllocator MemoryAllocator::Create(const Device& device) const {
    VmaAllocatorCreateInfo allocator_info {};
    allocator_info.physicalDevice = device.GetGPU();
    allocator_info.device = device.Get();
    VmaAllocator allocator;
    ErrorCheck(vmaCreateAllocator(&allocator_info, &allocator));
    return allocator;
}
}; // renderer::vlk