#pragma once
#include <string>
#include "vulkan_shared.h"
#include "device.h"

namespace renderer::vlk {
// Reads shader file and creates shader module
// Shader module can be deleted after pipeline is created
// Shader module is not tied to anything specific - like shader type or shader entry point
class ShaderModule {
public:
    // File must be SPIR-V file
    ShaderModule(const VkDevice& device,  const std::string& file_name);
    ~ShaderModule();
    ShaderModule(ShaderModule const&) = delete;
    ShaderModule operator=(ShaderModule const&) = delete;
    const VkShaderModule& Get() const;
private:
    // Loads and returns binary data from given file
    // Used to load SPIR-V data
    static std::vector<char> ReadBinaryFile(const std::string& file_name);
    VkShaderModule Create(const VkDevice& device, const std::string& file_name) const;
    // We grab reference and store it here because we need it in destructor
    const VkDevice& device_;
    // For logging
    const std::string file_name_;
    const VkShaderModule shader_module_ = VK_NULL_HANDLE;
};
}; // renderer vlk