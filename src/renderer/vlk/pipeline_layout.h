#pragma once
#include <vector>
#include "vulkan_shared.h"
#include "device.h"

// Describes shader resource layout in pipeline
// This object is needed when creating pipeline and when binding descriptor set to command
// buffer (to establish compatability)
namespace renderer::vlk {
class PipelineLayout {
public:
    // Pipeline layout configuration
    struct Params {
        // Describes descriptors that are to be used in pipeline
        // Resources with handles VkDescriptorSetLayout should stick around while pipeline is used
        std::vector<VkDescriptorSetLayout> layouts;
        // ... TODO push constants here
    };
    PipelineLayout(const VkDevice& device, const Params& params);
    ~PipelineLayout();
    PipelineLayout(PipelineLayout const&) = delete;
    PipelineLayout operator=(PipelineLayout const&) = delete;
    const VkPipelineLayout& Get() const;
private:
    VkPipelineLayout Create(const Params& params) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
};
};  // renderer vlk