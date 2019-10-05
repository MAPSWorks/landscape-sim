#pragma once
#include <vector>
#include <base/types.h>
#include "vulkan_shared.h"

// Stores a descriptor set that is created from given descriptor pool.
// Descriptor set is a collection of descriptors.
// Set will be destroyed together with descriptor pool destruction.
namespace renderer::vlk {
class DescriptorSet {
public:
    // pool - descriptor pool to allocate from.
    // layout - layouts ths set is going to be based on (descriptor set layout is kinda like type)
    DescriptorSet(const VkDevice& device, const VkDescriptorPool& pool, const VkDescriptorSetLayout& layout);
    // Get descriptor set with given index
    const VkDescriptorSet& Get() const;
    // Update descriptor set with given uniform buffer
    void UpdateUniformBuffer(const VkBuffer& buffer, t::U64 buffer_size) const;
private:
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    VkDescriptorSet Allocate(const VkDescriptorPool& pool, const VkDescriptorSetLayout& layout) const;
    const VkDescriptorSet descriptor_set_;
};
}; // renderer vlk