//
// Created by Ivars Rusbergs in 2021
//
// Physical and logical graphics device abstraction
#ifndef LSIM_RENDERER_VLK_DEVICE_H_
#define LSIM_RENDERER_VLK_DEVICE_H_

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// To get reference to physical device call GetGPU()
// To get reference to logical device call Get()
class Device {
public:
  Device(const VkInstance& instance);
  ~Device();
  // Returns reference to Vulkan physical device object
  const VkPhysicalDevice &GetGPU() const;

private:
  VkPhysicalDevice AcquirePhysicalDevice(const VkInstance& instance) const;
  void PrintPhysicalDeviceProperties(const VkPhysicalDevice& gpu) const;
  // The selected device Vulkan is going to use, aka physical device
  const VkPhysicalDevice gpu_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif