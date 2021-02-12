//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of Vulkan shader module object.
// Includes reading in binary spir-v file.
#ifndef LSIM_RENDERER_VLK_SHADER_NODULE_H_
#define LSIM_RENDERER_VLK_SHADER_NODULE_H_
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Reads in shader file and creates shader module.
// Shader module can be deleted after pipeline is created.
// Shader module is not tied to anything specific - like shader type or shader
// entry point.
class ShaderModule {
public:
  ShaderModule(const VkDevice &device, const std::string &file_path);
  ~ShaderModule();
  ShaderModule(ShaderModule const &) = delete;
  ShaderModule operator=(ShaderModule const &) = delete;
  // Returns Vulkan object handle
  const VkShaderModule &Handle() const;

private:
  VkShaderModule Create(const std::string &file_name) const;
  // Reference to resource this object is created with
  const VkDevice &device_;
  // To store path to file this module was created from
  const std::string file_path_;
  const VkShaderModule shader_module_ = VK_NULL_HANDLE;
};
// Free-standing functions
// Loads and returns binary data from given file
std::vector<char> ReadBinaryFile(const std::string &file_path);
} // namespace lsim::renderer::vlk

#endif