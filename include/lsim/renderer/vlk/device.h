//
// Created by Ivars Rusbergs in 2021
//
// Physical and logical graphics device abstraction together with device queue handles
#ifndef LSIM_RENDERER_VLK_DEVICE_H_
#define LSIM_RENDERER_VLK_DEVICE_H_
#include <vulkan/vulkan.h>

#include "device_queue.h"

namespace lsim::renderer::vlk {
// To get reference to physical device call GetGPU()
// To get reference to logical device call Get()
class Device {
public:
  Device(const VkInstance &instance);
  ~Device();
  Device(Device const &) = delete;
  Device operator=(Device const &) = delete;
  // Returns reference to Vulkan physical device object
  const VkPhysicalDevice &GetGPU() const;

private:
  VkPhysicalDevice AcquirePhysicalDevice(const VkInstance &instance) const;
  void PrintPhysicalDeviceProperties(const VkPhysicalDevice &gpu) const;
  // The selected device Vulkan is going to use, aka physical device
  const VkPhysicalDevice gpu_ = VK_NULL_HANDLE;
  // Device queue handles and selection.
  // Since queues are logically tied to device, they belong here.
  DeviceQueue queue_;
};
} // namespace lsim::renderer::vlk

#endif