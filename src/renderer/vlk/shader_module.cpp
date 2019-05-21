#include "shader_module.h"
#include <fstream>
#include <base/log.h>

namespace renderer::vlk {
ShaderModule::ShaderModule(const VkDevice& device, const std::string& file_name) :
    device_(device),
    file_name_(file_name),
    shader_module_(Create(file_name)) {
    base::Log::Info("Renderer: shader module '",file_name_,"' created");
}

ShaderModule::~ShaderModule() {
    base::Log::Info("Renderer: shader module '", file_name_, "' destroying...");
    vkDestroyShaderModule(device_, shader_module_, nullptr);
}

const VkShaderModule& ShaderModule::Get() const {
    return shader_module_;
}

// Static
// Loads and returns binary data from given file
// Used to load SPIR-V data
std::vector<char> ShaderModule::ReadBinaryFile(const std::string& file_name) {
    std::ifstream file(file_name, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Renderer:: failed to open file " + file_name);
    }
    // We are at the back so we can tell size of a file and allocate buffer
    size_t file_size = (size_t)file.tellg();
    std::vector<char> buffer(file_size);
    // Go to beginning and read all at once
    file.seekg(0);
    file.read(buffer.data(), file_size);
    file.close();
    base::Log::Info("Renderer: ",buffer.size()," bytes read from '", file_name);
    return buffer;
}

// The compilation of the spir-v code to GPU machine code doesn't happen until the graphics pipeline
// is created. That's why we can delete these shader modules after pipeline is created
VkShaderModule ShaderModule::Create(const std::string& file_name) const {
    // It is ok that this buffer is destroyed at exiting function
    const auto spirv_data = ReadBinaryFile(file_name);
    VkShaderModuleCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = spirv_data.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(spirv_data.data());
    VkShaderModule shader_module;
    ErrorCheck(vkCreateShaderModule(device_, &create_info, nullptr, &shader_module));
    return shader_module;
}
}; // renderer vlk
