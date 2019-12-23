#pragma once
#include <vector>
//#include <string>
#include <vulkan/vulkan.h>

// Graphics vulkan commonly used types
namespace renderer::vlk {
// Types
using ExtentionVector = std::vector<const char*>;
using LayerVector = std::vector<const char*>;
using BufferSize = VkDeviceSize;
using QueueFamilyIndex = uint32_t;
// Redefine vulkan values to decouple Vulkan and context where we use this library
enum class ShaderStage {
    kVertex = VK_SHADER_STAGE_VERTEX_BIT,
    kFragment = VK_SHADER_STAGE_FRAGMENT_BIT,
    kAllGraphics = VK_SHADER_STAGE_ALL_GRAPHICS
};
// Define bitwise or for ShaderStages
inline ShaderStage operator |(ShaderStage lhs, ShaderStage rhs) {
    return static_cast<ShaderStage> (
            static_cast<VkShaderStageFlagBits>(lhs) |
            static_cast<VkShaderStageFlagBits>(rhs)
            );
}

enum class DescriptorType {
    kUniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    kSampler = VK_DESCRIPTOR_TYPE_SAMPLER,
    kCombinedImageSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    kSampledImage = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
    kStorageImage = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    kUniformTexelBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
    kStorageTexelBuffer = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
    kStorageBuffer = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    kUniformBufferDynamic = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
    kStorageBufferDynamic = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
    kInputAttachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
};
};
