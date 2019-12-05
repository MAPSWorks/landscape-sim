#pragma once
#include "vulkan_shared.h"

// Sampler provides interface to extract colors from image in shader
namespace renderer::vlk {
class Sampler {
public:
    // Common usage modes for given sampler.
    // Given usage mode uses different sampler create prameter combination.
    enum class UsageMode {
        // The most common usage as a texture (default value)
        kDefault,
        // Used as a source for height data in shader
        kHeightmap
    };
    Sampler(const VkDevice& device, UsageMode usage_mode = UsageMode::kDefault);
    ~Sampler();
    Sampler(Sampler const&) = delete;
    Sampler operator=(Sampler const&) = delete;
    const VkSampler& Get() const;
private:
    VkSampler Create(UsageMode usage_mode) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkSampler sampler_ = VK_NULL_HANDLE;
};

}; //renderer::vlk