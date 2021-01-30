//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan application instance object
// Instance stores all aplication-wide Vulkan state
#ifndef LSIM_RENDERER_VLK_INSTANCE_H_
#define LSIM_RENDERER_VLK_INSTANCE_H_
#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class Instance {
public:
  // Alias for extension vector data ttype
  using ExtVector = std::vector<const char *>;
  // extensions is a vector of required extensions provided by OS.
  Instance(ExtVector extensions);
  ~Instance();
  Instance(Instance const &) = delete;
  Instance operator=(Instance const &) = delete;
  const VkInstance &Get() const;

private:
  // Recieves array of required extensions and appends other.
  // Returns updated extension vector.
  ExtVector AppendExtensions(ExtVector extensions) const;
  VkInstance Create(const ExtVector &extensions) const;
  ExtVector extensions_;
  const VkInstance instance_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif