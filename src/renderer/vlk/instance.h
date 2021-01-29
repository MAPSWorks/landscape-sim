//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan application instance object
// Instance stores all aplication-wide Vulkan state
#include <vector>

#ifndef LSIM_RENDERER_VLK_INSTANCE_H_
#define LSIM_RENDERER_VLK_INSTANCE_H_
#include "vulkan_shared.h"

namespace renderer::vlk {
class Instance {
public:
  Instance(const std::vector<const char *> &extensions);
  ~Instance();
  Instance(Instance const &) = delete;
  Instance operator=(Instance const &) = delete;
  const VkInstance& Get() const;
private:
  VkInstance Create(const std::vector<const char *> &extensions) const;
  const VkInstance instance_ = VK_NULL_HANDLE;
};
}; // namespace renderer::vlk

#endif