#pragma once
#include <base/types.h>
#include "vulkan_shared.h"
#include "device.h"

// Describe what descriptor bindings are going to be used in what shader stages
namespace renderer::vlk {
class DescriptorSetLayout {
public:
    enum class DescriptorType {
        kUniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
    };
    // Describes the descriptor set binding
    struct Binding {
        // corresponds to layout(binding = n)  in shader
        t::U32 index;
        // Type of descriptor layout. Uniform buffer, image sampler etc
        DescriptorType type;
        // In what stage it descriptor is going to be accessible
        ShaderStage stage;
        // Number of descriptors contained in the binding, accessed in a shader as an array.
        t::U32 count = 1;
        // Comparison operator used in cache to compare if this binding already exists.
        // NOTE: add to this function if adding new members
        bool operator==(const Binding& other) const {
            return index == other.index && type == other.type && stage == other.stage && count == other.count;
        }
    };
    DescriptorSetLayout(const VkDevice& device, const std::vector<Binding>& bindings);
    ~DescriptorSetLayout();
    DescriptorSetLayout(DescriptorSetLayout const&) = delete;
    DescriptorSetLayout operator=(DescriptorSetLayout const&) = delete;
    const VkDescriptorSetLayout& Get() const;
    const std::vector<Binding>& GetBindings() const;
private:
    VkDescriptorSetLayout Create(const std::vector<Binding>& bindings) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    // Bindings with which the descriptor set layout was created
    const std::vector<Binding> bindings_;
    const VkDescriptorSetLayout layout_ = VK_NULL_HANDLE;
};
}; // renderer vlk