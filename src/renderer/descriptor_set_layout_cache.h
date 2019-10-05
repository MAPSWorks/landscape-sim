#pragma once
#include <vector>
#include <unordered_map> 
#include <memory>
#include "vlk/types.h"
#include "vlk/device.h"
#include "vlk/descriptor_set_layout.h"

// This class stores descriptor set layouts as a cahce.
// Descriptor set layout specifies how resources are accessed in shaders.
// Many objects can have the same layout descriptions and there is a need
// for comparison and reuseage if description already is used somewhere.
// Additionally descriptor layout type total count and set total count are tracked,
// because using cache makes this information impossible to retrieve after. 
// NOTE: It should be created before pipelines and destroyed after pipelines are destroyed.
namespace renderer {
class DescriptorSetLayoutCache {
public:
    // Gather stats of how many descriptors of what type are created and 
    // how many total sets are created.
    // This is needed later for descriptor pool creation.
    // NOTE: We could later gather this info from all descriptor set layouts and bindings
    // but we use cache so we dont store duplicates for same type layouts, but we need this info for
    // actual descriptor set allocation from pool - we dont duplicate layouts but we duplicate sets.
    struct DescriptorSetStats {
        // For each descriptor type store how many of them are to be created
        std::unordered_map<vlk::DescriptorType, t::U32> descriptor_type_count;
        // And totoal count of set of descriptors
        t::U32 set_count = 0;
    };
    DescriptorSetLayoutCache(const VkDevice& device);
    // If descriptor set layout already exists with such bindings return existing layout reference.
    // If descriptor set layout doesn't exist with such bindings, add it and return the layout reference
    const vlk::DescriptorSetLayout& AddDescriptorSetLayout(const std::vector<vlk::DescriptorSetLayout::Binding>& bindings);
    const DescriptorSetStats& GetDescrikptorSetStats() const;
private:
    void UpdateDescriptorSetStats(const std::vector<vlk::DescriptorSetLayout::Binding>& bindings);
    // Store reference to device to be used for object creation later
    const VkDevice& device_;
    // How many descriptors of what type are created and how many sets
    // Needed for descriptor pool.
    DescriptorSetStats descriptor_set_stats_;
    // Layout collection
    // Unique ptr because the class has no move or copy constructors
    std::vector<std::unique_ptr<vlk::DescriptorSetLayout>> descriptor_set_layouts_;
};
}; // renderer