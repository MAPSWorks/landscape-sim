#include "sampler.h"
#include <stdexcept>
#include <base/log.h>

namespace renderer::vlk {
Sampler::Sampler(const VkDevice& device, UsageMode usage_mode) :
    device_(device),
    sampler_(Create(usage_mode)) {
    base::Log::Info("Renderer: sampler created");
}

Sampler::~Sampler() {
    base::Log::Info("Renderer: sampler destroying...");
    vkDestroySampler(device_, sampler_, nullptr);
}

const VkSampler& Sampler::Get() const {
    return sampler_;
}

VkSampler Sampler::Create(UsageMode usage_mode) const {
    VkSamplerCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    // Common parameters equal for every usage mode
    create_info.magFilter = VK_FILTER_LINEAR;
    create_info.minFilter = VK_FILTER_LINEAR;
    create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    create_info.compareEnable = VK_FALSE;
    create_info.compareOp = VK_COMPARE_OP_ALWAYS;
    create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    create_info.unnormalizedCoordinates = VK_FALSE;
    // Switch creation parameters dependant on usage mode
    switch (usage_mode) {
    // Most common usage of a sampler - simple texture
    case UsageMode::kDefault:
        create_info.anisotropyEnable = VK_TRUE;
        create_info.maxAnisotropy = 16;
        break;
    // Sampler used for height data reading in shader
    case UsageMode::kHeightmap:
        create_info.anisotropyEnable = VK_FALSE;
        break;
    default:
        throw std::runtime_error("Renderer: sampler usage mode unknown");
        break;
    }
    VkSampler sampler;
    ErrorCheck(vkCreateSampler(device_, &create_info, nullptr, &sampler));
    return sampler;
}
}; //renderer::vlk