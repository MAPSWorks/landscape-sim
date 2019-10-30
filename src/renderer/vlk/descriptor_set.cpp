#include "descriptor_set.h"
#include <base/log.h>
#include <deque>

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

void DescriptorSet::UpdateUniformBuffer(const VkBuffer& buffer, t::U64 buffer_size) const {
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = buffer; 
    buffer_info.offset = 0;
    // Update with given buffer size, except when passed 0 - use whole size
    buffer_size == 0 ? buffer_info.range = VK_WHOLE_SIZE : buffer_info.range = buffer_size;
    VkWriteDescriptorSet descriptor_write{};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = descriptor_set_;
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo = &buffer_info;
    vkUpdateDescriptorSets(device_, 1, &descriptor_write, 0, nullptr);
    
    base::Log::Info("Renderer: descriptor set updated with buffer of size (", buffer_size, "). '0' means whole size");
}

void DescriptorSet::Update(const std::vector<ResourcesToUpdate>& resources_to_update) const {
    std::vector<VkWriteDescriptorSet> descriptor_writes;
    // Since descriptor_writes holds pointers to other structures
    // we need to store also those structures until vulkan call is executed.
    // Use deque instead of vector because new object insertion does not invalidate 
    // pointers to elements (unlike vector).
    std::deque<VkDescriptorBufferInfo> buffer_infos;
    std::deque<VkDescriptorImageInfo> image_infos;
    // Foreach resource to update
    for (size_t i = 0; i < resources_to_update.size(); ++i) {
        const auto& resource = resources_to_update.at(i);
        VkWriteDescriptorSet descriptor_write {};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = descriptor_set_;
        // TODO: maybe this index is (location=n) in shader?
        // Index in bindings array
        descriptor_write.dstBinding = static_cast<t::U32>(i);
        // For uniform buffers
        if (resource.type == DescriptorType::kUniformBuffer) {
            VkDescriptorBufferInfo buffer_info {};
            buffer_info.buffer = resource.buffer;
            buffer_info.offset = resource.buffer_offset;
            // Update with given buffer size, except when passed 0 - use whole size
            resource.buffer_range == 0 ? buffer_info.range = VK_WHOLE_SIZE : buffer_info.range = resource.buffer_range;
            buffer_infos.push_back(buffer_info);
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptor_write.descriptorCount = resource.count;
            descriptor_write.pBufferInfo = &buffer_infos.back();
            base::Log::Info("Renderer: descriptor set to be updated with uniform buffer resource of size -", 
                resource.buffer_range, " ('0' means whole size)");
        } else
        // For combined image sampler
        if (resource.type == DescriptorType::kCombinedImageSampler) {
            VkDescriptorImageInfo image_info {};
            image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            image_info.imageView = resource.image_view;
            image_info.sampler = resource.image_sampler;
            image_infos.push_back(image_info);
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptor_write.descriptorCount = resource.count;
            descriptor_write.pImageInfo = &image_infos.back();
            base::Log::Info("Renderer: descriptor set to be updated with comb. image sampler resource");
        }
        // TODO: add other resource types here
        else {
            throw std::runtime_error("Renderer: unrecognized descriptor type tried to update descriptor set");
        }
        descriptor_writes.push_back(descriptor_write);
    }
    vkUpdateDescriptorSets(device_, static_cast<t::U32>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
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