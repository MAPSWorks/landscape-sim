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
enum class DescriptorType {
    kUniformBuffer = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
};
};
