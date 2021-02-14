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
  const VkPhysicalDevice &Handle() const;

private:
  // Checks if gicen GPU satisfies requirements for this engine
  static bool Suitable(const VkPhysicalDevice &physical_device,
                       const VkSurfaceKHR &surface);
  static void PrintProperties(const VkPhysicalDevice &physical_device);
  // Select physical device the engine is going to se
  VkPhysicalDevice Acquire(const VkInstance &instance,
                           const VkSurfaceKHR &surface) const;
  // The selected device the engine is going to use, aka physical device
  const VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif