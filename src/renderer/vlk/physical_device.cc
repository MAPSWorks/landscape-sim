//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/physical_device.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/queue_families.h"
#include "lsim/renderer/vlk/swapchain.h"

namespace lsim::renderer::vlk {
PhysicalDevice::PhysicalDevice(const VkInstance &instance, const VkSurfaceKHR &surface)
    : physical_device_(Acquire(instance, surface)) {
  PrintProperties(physical_device_);
}

const VkPhysicalDevice &PhysicalDevice::Handle() const { return physical_device_; }

// Physical device is not created but acquired and need not be deleted
VkPhysicalDevice PhysicalDevice::Acquire(const VkInstance &instance,
                              const VkSurfaceKHR &surface) const {
  // List of all physical devices available
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
  if (!device_count) {
    throw std::runtime_error(
        "renderer: failed to find GPU with Vulkan support");
  }
  std::vector<VkPhysicalDevice> physical_devices(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data());
  // Choose device that satisfies engine requirements
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  for (auto device : physical_devices) {
    if (Suitable(device, surface)) {
      physical_device = device;
      break;
    }
  }
  if (physical_device == VK_NULL_HANDLE) {
    throw std::runtime_error(
        "renderer: failed to find rdevice with required features");
  }

  return physical_device;
}

// static
bool PhysicalDevice::Suitable(const VkPhysicalDevice &physical_device, const VkSurfaceKHR &surface) {
  const QueueFamilies queue_families(physical_device, surface);
  const auto swapchain_support = Swapchain::QuerySupport(physical_device, surface);
  return queue_families.Complete() && swapchain_support.IsCapable();
}

// static
void PhysicalDevice::PrintProperties(const VkPhysicalDevice &physical_device) {
  // Log some properties
  VkPhysicalDeviceProperties gpu_properties;
  vkGetPhysicalDeviceProperties(physical_device, &gpu_properties);
  base::Log::Info("renderer", "GPU picked -", gpu_properties.deviceName,
                  ", Vulkan v. -", VK_VERSION_MAJOR(gpu_properties.apiVersion),
                  ".", VK_VERSION_MINOR(gpu_properties.apiVersion), ".",
                  VK_VERSION_PATCH(gpu_properties.apiVersion));
}
} // namespace lsim::renderer::vlk