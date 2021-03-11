//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan physical device object.
#ifndef LSIM_RENDERER_VLK_PHYSICAL_DEVICE_H_
#define LSIM_RENDERER_VLK_PHYSICAL_DEVICE_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class PhysicalDevice {
public:
  PhysicalDevice(const VkInstance &instance, const VkSurfaceKHR &surface);
  [[nodiscard]] VkPhysicalDevice Handle() const;

private:
  // The selected device the engine is going to use, aka physical device
  VkPhysicalDevice const physical_device_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif