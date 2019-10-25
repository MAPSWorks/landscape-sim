#pragma once
#include "vulkan_shared.h"

// Sampler provides interface to extract colors from image in shader
namespace renderer::vlk {
class Sampler {
public:
    Sampler(const VkDevice& device);
    ~Sampler();
    Sampler(Sampler const&) = delete;
    Sampler operator=(Sampler const&) = delete;
    const VkSampler& Get() const;
private:
    VkSampler Create() const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkSampler sampler_ = VK_NULL_HANDLE;
};

}; //renderer::vlk