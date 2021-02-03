//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/device.h"

#include <stdexcept>
#include <vector>

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/device_queue.h"

namespace lsim::renderer::vlk {
Device::Device(const VkInstance &instance)
    : gpu_(AcquireGPU(instance)), queue_(gpu_) {

  base::Log::Info("renderer", "device", "created");
}

Device::~Device() { base::Log::Info("renderer", "device", "destroying.."); }

const VkPhysicalDevice &Device::GetGPU() const { return gpu_; }

// Physical device is not created but acquired and need not be deleted
VkPhysicalDevice Device::AcquireGPU(const VkInstance &instance) const {
  // List of all physical devices available
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
  if (!device_count) {
    throw std::runtime_error(
        "renderer: failed to find GPU with Vulkan support");
  }
  // List of the GPU's available
  std::vector<VkPhysicalDevice> physical_devices(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data());

  // Choose device that satisfies engine requirements
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  for (auto device : physical_devices) {
    if (IsSuitableGPU(device)) {
      physical_device = device;
    }
  }
  if (physical_device == VK_NULL_HANDLE) {
    throw std::runtime_error(
        "renderer: failed to find rdevice with required features");
  }

  PrintGPUProperties(physical_device);
  return physical_device;
}

bool Device::IsSuitableGPU(const VkPhysicalDevice &gpu) const {
  const auto queue_family = DeviceQueue::SelectFamilies(gpu);
  return queue_family.IsComplete();
}

void Device::PrintGPUProperties(const VkPhysicalDevice &gpu) const {
  // Log some properties
  VkPhysicalDeviceProperties p_device_properties;
  vkGetPhysicalDeviceProperties(gpu, &p_device_properties);
  base::Log::Info("renderer", "GPU picked -", p_device_properties.deviceName,
                  ", Vulkan v. -",
                  VK_VERSION_MAJOR(p_device_properties.apiVersion), ".",
                  VK_VERSION_MINOR(p_device_properties.apiVersion), ".",
                  VK_VERSION_PATCH(p_device_properties.apiVersion));
}

} // namespace lsim::renderer::vlk