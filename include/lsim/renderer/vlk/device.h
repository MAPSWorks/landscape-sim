//
// Created by Ivars Rusbergs in 2021
//
// Physical and logical graphics device abstraction together with device queue
// handles
#ifndef LSIM_RENDERER_VLK_DEVICE_H_
#define LSIM_RENDERER_VLK_DEVICE_H_
#include <vector>

#include <vulkan/vulkan.h>

#include "device_queue.h"

namespace lsim::renderer::vlk {
// Contains both logical and physical device handles.
// Physical device (gpu) are acquired and need not be destroyed.
// Logical device is created and destroyed.
// To get reference to physical device call GetGPU().
// To get reference to logical device call Get().
class Device {
public:
  Device(const VkInstance &instance);
  ~Device();
  Device(Device const &) = delete;
  Device operator=(Device const &) = delete;
  // Returns reference to Vulkan physical device object
  const VkPhysicalDevice &GetGPU() const;
  // Returns reference to Vulkan logical device object
  const VkDevice &Get() const;

private:
  VkPhysicalDevice AcquireGPU(const VkInstance &instance) const;
  void PrintGPUProperties(const VkPhysicalDevice &gpu) const;
  // Checks if gicen GPU is suitable for this engine
  bool IsSuitableGPU(const VkPhysicalDevice &gpu) const;
  // Creates and return handle to logical device from the given gpu
  VkDevice CreateDevice(const VkPhysicalDevice &gpu) const;
  // Retrieve queue handle from logical device
  VkQueue GetGraphicsQueue() const;
  // VkQueue GetPresentQueue() const;
  // Device extentions that the logical device has to support
  const std::vector<const char *> required_extentions_;
  // The selected device Vulkan is going to use, aka physical device
  const VkPhysicalDevice gpu_ = VK_NULL_HANDLE;
  // Device queue handles and selection.
  // Since queues are logically tied to device, they belong here.
  DeviceQueue queue_;
  // Logical device can be thought of as physical device with enabled certain
  // features
  const VkDevice device_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif