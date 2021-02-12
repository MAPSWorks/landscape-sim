//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/device.h"

#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/device_queue.h"
#include "lsim/renderer/vlk/swapchain.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
Device::Device(const VkInstance &instance, const VkSurfaceKHR &surface)
    : required_extentions_({VK_KHR_SWAPCHAIN_EXTENSION_NAME}),
      gpu_(AcquireGPU(instance, surface)), queue_(gpu_, surface),
      device_(CreateDevice(gpu_)) {
  // Retrieve queues from device and set their handles for DeviceQueue class
  queue_.SetGraphics(GetGraphicsQueue());
  queue_.SetPresent(GetPresentQueue());
  base::Log::Info("renderer", "device", "created");
}

Device::~Device() {
  base::Log::Info("renderer", "device", "destroying..");
  // Destroys logical device and queues created from logical device
  vkDestroyDevice(device_, nullptr);
}

const VkPhysicalDevice &Device::GPU() const { return gpu_; }

const VkDevice &Device::Handle() const { return device_; }

const DeviceQueue &Device::Queue() const { return queue_; }

// Physical device is not created but acquired and need not be deleted
VkPhysicalDevice Device::AcquireGPU(const VkInstance &instance,
                                    const VkSurfaceKHR &surface) const {
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
    if (IsSuitableGPU(device, surface)) {
      physical_device = device;
      break;
    }
  }
  if (physical_device == VK_NULL_HANDLE) {
    throw std::runtime_error(
        "renderer: failed to find rdevice with required features");
  }

  PrintGPUProperties(physical_device);
  return physical_device;
}

bool Device::IsSuitableGPU(const VkPhysicalDevice &gpu,
                           const VkSurfaceKHR &surface) const {
  const auto queue_family = DeviceQueue::SelectFamilies(gpu, surface);
  const auto swapchain_support = Swapchain::QuerySupport(gpu, surface);
  return queue_family.IsComplete() && swapchain_support.IsCapable();
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

VkDevice Device::CreateDevice(const VkPhysicalDevice &gpu) const {
  const auto queue_create_infos = queue_.CreateInfos();
  // Device features to enable
  VkPhysicalDeviceFeatures device_features{};
  device_features.fillModeNonSolid = VK_TRUE;
  // Enable anisotropic filtering
  device_features.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  device_create_info.queueCreateInfoCount =
      static_cast<uint32_t>(queue_create_infos.size());
  device_create_info.pEnabledFeatures = &device_features;
  device_create_info.enabledExtensionCount =
      static_cast<uint32_t>(required_extentions_.size());
  device_create_info.ppEnabledExtensionNames = required_extentions_.data();

  VkDevice device;
  ErrorCheck(vkCreateDevice(gpu, &device_create_info, nullptr, &device));
  return device;
}

VkQueue Device::GetGraphicsQueue() const {
  VkQueue queue;
  vkGetDeviceQueue(device_, queue_.Families().graphics.value(), 0,
                   &queue);
  return queue;
}

VkQueue Device::GetPresentQueue() const {
  VkQueue queue;
  vkGetDeviceQueue(device_, queue_.Families().present.value(), 0,
                   &queue);
  return queue;
}

} // namespace lsim::renderer::vlk