#include "descriptor_set_layout.h"
#include <base/log.h>

namespace renderer::vlk {
DescriptorSetLayout::DescriptorSetLayout(const VkDevice& device, const std::vector<Binding>& bindings) :
    device_(device),
    layout_(Create(bindings)) {
    base::Log::Info("Renderer: descriptor set layout created");
}

DescriptorSetLayout::~DescriptorSetLayout() {
    base::Log::Info("Renderer: descriptor set layout destroying...");
    vkDestroyDescriptorSetLayout(device_, layout_, nullptr);
}

const VkDescriptorSetLayout& DescriptorSetLayout::Get() const {
    return layout_;
}

// bindings - array of bindings of descriptors in shader stages
VkDescriptorSetLayout DescriptorSetLayout::Create(const std::vector<Binding>& bindings) const {
    std::vector<VkDescriptorSetLayoutBinding> layout_bindings;
    for (const auto& binding : bindings) {
        VkDescriptorSetLayoutBinding layout_binding {};
        layout_binding.binding = binding.index;
        layout_binding.descriptorType = static_cast<VkDescriptorType>(binding.type);
        layout_binding.descriptorCount = binding.count;
        layout_binding.pImmutableSamplers = nullptr;
        layout_binding.stageFlags = static_cast<VkShaderStageFlagBits>(binding.stage);
        layout_bindings.push_back(layout_binding);
    }
    VkDescriptorSetLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = static_cast<t::U32>(layout_bindings.size());
    layout_info.pBindings = layout_bindings.data();
    VkDescriptorSetLayout descriptor_set_layout;
    ErrorCheck(vkCreateDescriptorSetLayout(device_, &layout_info, nullptr, &descriptor_set_layout));
    base::Log::Info("Renderer: ", layout_info.bindingCount," bindings in layout set");
    return descriptor_set_layout;
}
}; // renderer vlk