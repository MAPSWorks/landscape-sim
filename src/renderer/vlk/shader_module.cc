//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/shader_module.h"
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {

ShaderModule::ShaderModule(const VkDevice &device, const std::string &file_path)
    : device_(device), file_path_(file_path),
      shader_module_(Create(file_path)) {
  base::Log::Info("renderer", "shader module", "created", file_path_);
}

ShaderModule::~ShaderModule() {
  base::Log::Info("renderer", "shader module", "destroying..", file_path_);
  vkDestroyShaderModule(device_, shader_module_, nullptr);
}

const VkShaderModule &ShaderModule::Handle() const { return shader_module_; }

VkShaderModule ShaderModule::Create(const std::string &file_name) const {
  // It is ok that this buffer is destroyed on exiting function
  const auto spirv_data = ReadBinaryFile(file_name);
  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = spirv_data.size();
  create_info.pCode = reinterpret_cast<const uint32_t *>(spirv_data.data());
  VkShaderModule shader_module;
  ErrorCheck(
      vkCreateShaderModule(device_, &create_info, nullptr, &shader_module));
  return shader_module;
}

std::vector<char> ReadBinaryFile(const std::string &file_path) {
  std::ifstream file(file_path, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("renderer: failed to open file " + file_path);
  }
  // We are at the back so we can tell size of a file and allocate buffer
  size_t file_size = (size_t)file.tellg();
  std::vector<char> buffer(file_size);
  // Go to beginning and read all at once
  file.seekg(0);
  file.read(buffer.data(), file_size);
  file.close();
  base::Log::Info("renderer", buffer.size(), "bytes read from", file_path);
  return buffer;
}
} // namespace lsim::renderer::vlk