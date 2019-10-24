#include "memory_allocator.h"
#include <base/log.h>
//#include "command_buffer.h"

namespace renderer::vlk {
MemoryAllocator::MemoryAllocator(const Device& device):
    device_(device),
    allocator_(Create(device_)){
    // if graphics is supported, transfer also implicitly is supported
   // transfer_command_pool_(device_.Get(), device_.GetQueue().GetFamilyIndices().graphics.value(), false, true) {
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
/*
// Source buffer must be host visible and transfer source, destination buffer must be transfer destination
void MemoryAllocator::CopyBuffer(const VkBuffer& src_buffer, const VkBuffer& dst_buffer, BufferSize size) const {
    CommandBuffer command_buffer(transfer_command_pool_);
    command_buffer.Begin(CommandBuffer::Usage::kOneTimeSubmit);
    command_buffer.CopyBuffer(src_buffer, dst_buffer, size);
    command_buffer.End();
    // Submit to graphics queue because graphics queue implicitly supports transfer
    device_.GetQueue().Submit(device_.GetQueue().GetGraphics(), command_buffer.Get());
    // Since we dont use fence
    ErrorCheck(vkQueueWaitIdle(device_.GetQueue().GetGraphics()));
    // Freeing is not ind command buffers destructor because it is not mandatory
    command_buffer.Free(device_.Get());
    base::Log::Info("Renderer: buffer copy performed. Size - ", size);
}*/
}; // renderer::vlk