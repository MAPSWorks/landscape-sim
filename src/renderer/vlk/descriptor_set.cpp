#include "descriptor_set.h"
#include <base/log.h>

namespace renderer::vlk {
DescriptorSet::DescriptorSet(const VkDevice& device, const VkDescriptorPool& pool,
    const VkDescriptorSetLayout& layout) :
    device_(device),
    descriptor_set_(Allocate(pool, layout)) {
    base::Log::Info("Renderer: descriptor set created");
}

const VkDescriptorSet& DescriptorSet::Get() const {
    return descriptor_set_;
}

void DescriptorSet::UpdateUniformBuffer(const VkBuffer& buffer, t::U32 buffer_size) const {
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = buffer;
    buffer_info.offset = 0;
    buffer_info.range = buffer_size;
    VkWriteDescriptorSet descriptor_write{};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = descriptor_set_;
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo = &buffer_info;
    vkUpdateDescriptorSets(device_, 1, &descriptor_write, 0, nullptr);
    
    base::Log::Info("Renderer: descriptor set updated with buffer of size (", buffer_size, ")");
}

VkDescriptorSet DescriptorSet::Allocate(const VkDescriptorPool& pool, const VkDescriptorSetLayout& layout) const {
    const std::vector<VkDescriptorSetLayout> layouts = { layout };
    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = pool;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = layouts.data();
    VkDescriptorSet descriptor_set;
    ErrorCheck(vkAllocateDescriptorSets(device_, &alloc_info, &descriptor_set));
    return descriptor_set;
}
}; // renderer vlk