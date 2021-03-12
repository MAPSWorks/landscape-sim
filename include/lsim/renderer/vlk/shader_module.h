//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of Vulkan shader module object.
// Includes reading in binary spir-v file.
#ifndef LSIM_RENDERER_VLK_SHADER_NODULE_H_
#define LSIM_RENDERER_VLK_SHADER_NODULE_H_
#include <string>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Reads in shader file and creates shader module.
// Shader module can be deleted after pipeline is created.
// Shader module is not tied to anything specific - like shader type or shader
// entry point.
class ShaderModule {
public:
  ShaderModule(VkDevice device, const std::string &file_path);
  ~ShaderModule();
  ShaderModule(ShaderModule const &) = delete;
  ShaderModule &operator=(ShaderModule const &) = delete;
  ShaderModule(ShaderModule &&) = delete;
  ShaderModule &operator=(ShaderModule &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkShaderModule Handle() const;

private:
  [[nodiscard]] VkShaderModule Create(const std::string &file_name) const;
  // Pointer to resource this object is created with
  VkDevice const context_device_;
  // To store path to file this module was created from
  const std::string file_path_;
  VkShaderModule const shader_module_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif