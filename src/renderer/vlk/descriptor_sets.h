#pragma once
#include <vector>
#include <base/types.h>
#include "vulkan_shared.h"

// Stores collection of descriptor sets that are created from given pool.
// Descriptor set is a collection of descriptors.
// Usually descriptor sets will be duplicated for each frame-in-flight.
// Sets will be destroyed together with descriptor pool destruction.
namespace renderer::vlk {
class DescriptorSets {
public:
    // pool - descriptor pool to allocate from.
    // layout - layout ths set is going to be based on (descriptor set layout is kinda like type)
    //          TODO: should be array?
    // set_count - descriptor set count to create (usually same as frames-in-flight count)
    DescriptorSets(const VkDevice& device, const VkDescriptorPool& pool, const VkDescriptorSetLayout& layout, 
        t::U32 set_count);
private:
    std::vector<VkDescriptorSet> Allocate(const VkDevice& device, const VkDescriptorPool& pool, 
        const VkDescriptorSetLayout& layout, t::U32 set_count) const;
    const std::vector<VkDescriptorSet> descriptor_sets_;
};
}; // renderer vlk