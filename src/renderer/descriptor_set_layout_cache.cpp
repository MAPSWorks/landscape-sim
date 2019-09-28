#include "descriptor_set_layout_cache.h"
#include <algorithm> 
#include <base/log.h>

namespace renderer {
DescriptorSetLayoutCache::DescriptorSetLayoutCache(const VkDevice& device) : 
    device_(device) {
    base::Log::Info("Renderer: descriptor set layout cache initialized");
}

// Add if doesnt exist
// Don't add if does exist
// Return the reference to object
const vlk::DescriptorSetLayout& DescriptorSetLayoutCache::AddDescriptorSetLayout(const std::vector<vlk::DescriptorSetLayout::Binding>& bindings) {
    // Search for descriptor that has the same binding descriptions as are passed through param
    const auto descriptor_iter = std::find_if(descriptor_set_layouts_.begin(), descriptor_set_layouts_.end(),
        [&bindings](const std::unique_ptr<vlk::DescriptorSetLayout>& obj)
        { return obj->GetBindings() == bindings; });
    // No such descriptor exists
    if (descriptor_iter == descriptor_set_layouts_.end()) {
        descriptor_set_layouts_.push_back(std::make_unique<vlk::DescriptorSetLayout>(device_, bindings));
        base::Log::Info("Renderer: new descriptor set layout added to cache");
        return *descriptor_set_layouts_.back();
    }
    // Descriptor already exists
    else {
        base::Log::Info("Renderer: no descriptor set layout was added to cache, existing one will be reused");
        return **descriptor_iter;
    }
}
};