//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/device.h"

#include <stdexcept>
#include <vector>

#include "lsim/base/log.h"

namespace lsim::renderer::vlk {
Device::Device(const VkInstance &instance)
    : gpu_(AcquirePhysicalDevice(instance)) {

  base::Log::Info("renderer", "device", "created");
}

Device::~Device() { base::Log::Info("renderer", "device", "destroying.."); }

const VkPhysicalDevice &Device::GetGPU() const { return gpu_; }

// Physical device is not created but acquired and need not be deleted
VkPhysicalDevice
Device::AcquirePhysicalDevice(const VkInstance &instance) const {
  // List of all physical devices available
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
  if (!device_count) {
    throw std::runtime_error(
        "renderer: failed to find GPU with Vulkan support");
  }
  std::vector<VkPhysicalDevice> physical_devices(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data());

  // TODO: pick physical device from the list here
  // For now pick first one in the list
  VkPhysicalDevice physical_device = physical_devices.at(0);
  PrintPhysicalDeviceProperties(physical_device);
  return physical_device;
}

void Device::PrintPhysicalDeviceProperties(const VkPhysicalDevice &gpu) const {
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