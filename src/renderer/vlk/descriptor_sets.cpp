#include "descriptor_sets.h"
#include <base/log.h>

namespace renderer::vlk {
DescriptorSets::DescriptorSets(const VkDevice& device, const VkDescriptorPool& pool, 
    const VkDescriptorSetLayout& layout, t::U32 set_count) :
    device_(device),
    descriptor_sets_(Allocate(pool, layout, set_count)) {
    base::Log::Info("Renderer: descriptor sets (", set_count,") created");
}

const VkDescriptorSet& DescriptorSets::Get(t::U32 index) const {
    return descriptor_sets_.at(index);
}

void DescriptorSets::UpdateBuffer(const std::vector<VkBuffer>& buffer, t::U32 buffer_size) const {
    for (size_t i = 0; i < descriptor_sets_.size(); i++) {
        VkDescriptorBufferInfo buffer_info {};
        buffer_info.buffer = buffer[i];
        buffer_info.offset = 0;
        buffer_info.range = buffer_size;
        VkWriteDescriptorSet descriptor_write {};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_sets_[i];
        descriptor_write.dstBinding = 0;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pBufferInfo = &buffer_info;
        vkUpdateDescriptorSets(device_, 1, &descriptor_write, 0, nullptr);
    }
    base::Log::Info("Renderer: descriptor sets updated for buffers (", buffer.size(), ")");
}

std::vector<VkDescriptorSet> DescriptorSets::Allocate(const VkDescriptorPool& pool,
    const VkDescriptorSetLayout& layout, t::U32 set_count) const {
    // Currently each set is meant for each frame in flight, therefore we duplicate them
    const std::vector<VkDescriptorSetLayout> layouts(set_count, layout);
    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pool;
    alloc_info.descriptorSetCount = set_count;
    alloc_info.pSetLayouts = layouts.data();
    std::vector<VkDescriptorSet> descriptor_sets(set_count);
    ErrorCheck(vkAllocateDescriptorSets(device_, &alloc_info, descriptor_sets.data()));
    return descriptor_sets;
}
}; // renderer vlk