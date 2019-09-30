#pragma once
#include <vector>
#include <base/types.h>
#include "vulkan_shared.h"
#include "device.h"

// Descriptor sets are allocated from descriptor pools.
// Many pools are used for threads. Each pool for each thread. 
// Define all decriptor types and their respective count to later allocate from this pool.
namespace renderer::vlk {
class DescriptorPool {
public:
    // Pool can be created for multiple descriptor types
    struct PoolSize {
        // Type of descriptors to be allocated from pool (uniform buffer, ...)
        DescriptorType  type;
        // Number of descriptors that will be allocated for given type
        t::U32 descriptor_count;
    };
    // pool_sizes - describes all types and size of descriptors that can be allocated from this pool
    // sets_max - maximum sets that can be allocated
    DescriptorPool(const VkDevice& device, const std::vector<PoolSize>& pool_sizes, t::U32 sets_max);
    ~DescriptorPool();
    DescriptorPool(DescriptorPool const&) = delete;
    DescriptorPool operator=(DescriptorPool const&) = delete;
    const VkDescriptorPool& Get() const;
private:
    VkDescriptorPool Create(const std::vector<PoolSize>& pool_sizes, t::U32 sets_max) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
};
}; // renderer vlk