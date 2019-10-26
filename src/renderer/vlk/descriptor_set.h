#pragma once
#include <vector>
#include <base/types.h>
#include "vulkan_shared.h"
#include "types.h"

// Stores a descriptor set that is created from given descriptor pool.
// Descriptor set is a collection of descriptors.
// Set will be destroyed together with descriptor pool destruction.
namespace renderer::vlk {
class DescriptorSet {
public:
    // Resource data that is passed to update (bind) to descriptor set
    struct ResourcesToUpdate {
        // Type of a descriptor within this set to update
        DescriptorType type;
        // How many descriptor of this type to update
        t::U32 count;
        // For buffers only
        VkBuffer buffer;
        // Offset for this buffer
        BufferSize buffer_offset;
        // Range to update (0 means whole)
        BufferSize buffer_range;
        // For image samplers only
        VkImageView image_view;
        VkSampler image_sampler;
    };
    // pool - descriptor pool to allocate from.
    // layout - layouts ths set is going to be based on (descriptor set layout is kinda like type)
    DescriptorSet(const VkDevice& device, const VkDescriptorPool& pool, const VkDescriptorSetLayout& layout);
    // Get descriptor set with given index
    const VkDescriptorSet& Get() const;
    // Update descriptor set with given uniform buffer
    // If buffer size is passed 0, use whole buffer size to bind
    void UpdateUniformBuffer(const VkBuffer& buffer, t::U64 buffer_size = 0) const;
    // Update this descriptor set with actual resources
    void Update(const std::vector<ResourcesToUpdate>& resources_to_update) const;
private:
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    VkDescriptorSet Allocate(const VkDescriptorPool& pool, const VkDescriptorSetLayout& layout) const;
    const VkDescriptorSet descriptor_set_;
};
}; // renderer vlk