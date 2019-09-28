#pragma once
#include <vector>
#include <memory>
#include "vlk/descriptor_set_layout.h"

// This class stores descriptor set layouts as a cahce.
// Descriptor set layout specifies how resources are accessed in shaders.
// Many objects can have the same layout descriptions and there is a need
// for comparison and reuseage if description already is used somewhere.
// NOTE: It should be created before pipelines and destroyed after pipelines are destroyed.
namespace renderer {
class DescriptorSetLayoutCache {
public:
    DescriptorSetLayoutCache(const VkDevice& device);
    // If descriptor set layout already exists with such bindings return existing layout reference.
    // If descriptor set layout doesn't exist with such bindings, add it and return the layout reference
    const vlk::DescriptorSetLayout& AddDescriptorSetLayout(const std::vector<vlk::DescriptorSetLayout::Binding>& bindings);
private:
    // Store reference to device to be used for object creation later
    const VkDevice& device_;
    // Layout collection
    // Unique ptr because the class has no move or copy constructors
    std::vector<std::unique_ptr<vlk::DescriptorSetLayout>> descriptor_set_layouts_;
};
}; // renderer