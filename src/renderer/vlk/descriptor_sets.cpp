#include "descriptor_sets.h"
#include <base/log.h>

namespace renderer::vlk {
DescriptorSets::DescriptorSets(const VkDevice& device, const VkDescriptorPool& pool, 
    const VkDescriptorSetLayout& layout, t::U32 set_count) :
    descriptor_sets_(Allocate(device, pool, layout, set_count)) {
    base::Log::Info("Renderer: descriptor sets (", set_count,") created");
}

std::vector<VkDescriptorSet> DescriptorSets::Allocate(const VkDevice& device, const VkDescriptorPool& pool,
    const VkDescriptorSetLayout& layout, t::U32 set_count) const {
    // Currently each set is meant for each frame in flight, therefore we duplicate them
    const std::vector<VkDescriptorSetLayout> layouts(set_count, layout);
    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pool;
    alloc_info.descriptorSetCount = set_count;
    alloc_info.pSetLayouts = layouts.data();
    std::vector<VkDescriptorSet> descriptor_sets(set_count);
    ErrorCheck(vkAllocateDescriptorSets(device, &alloc_info, descriptor_sets.data()));
    return descriptor_sets;
}
}; // renderer vlk