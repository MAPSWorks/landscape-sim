#include "descriptor_pool.h"
#include <base/log.h>

namespace renderer::vlk {
DescriptorPool::DescriptorPool(const VkDevice& device, const std::vector<PoolSize>& pool_sizes, t::U32 sets_max) :
    device_(device),
    descriptor_pool_(Create(pool_sizes, sets_max)) {
    base::Log::Info("Renderer: descriptor pool created for ", pool_sizes.size(), " descriptor types");
}

DescriptorPool::~DescriptorPool() {
    base::Log::Info("Renderer: descriptor pool destroying...");
    vkDestroyDescriptorPool(device_, descriptor_pool_, nullptr);
}

const VkDescriptorPool& DescriptorPool::Get() const {
    return descriptor_pool_;
}

VkDescriptorPool DescriptorPool::Create(const std::vector<PoolSize>& pool_sizes, t::U32 sets_max) const {
    // Define descriptor types and how many to allocate one-by-one
    std::vector<VkDescriptorPoolSize> descriptor_pool_sizes;
    for (const auto& pool_size : pool_sizes) {
        VkDescriptorPoolSize descriptor_pool_size{};
        // What type of descriptors
        descriptor_pool_size.type = static_cast<VkDescriptorType>(pool_size.type);
        // How many descriptors of this type
        descriptor_pool_size.descriptorCount = pool_size.descriptor_count;
        descriptor_pool_sizes.push_back(descriptor_pool_size);
    }
    VkDescriptorPoolCreateInfo pool_create_info {};
    pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_create_info.poolSizeCount = static_cast<t::U32>(descriptor_pool_sizes.size());
    pool_create_info.pPoolSizes = descriptor_pool_sizes.data();
    // Maximum sets that can be allocated
    pool_create_info.maxSets = sets_max;
    VkDescriptorPool  pool;
    ErrorCheck(vkCreateDescriptorPool(device_, &pool_create_info, nullptr, &pool));
    return pool;
}
}; // renderer vlk