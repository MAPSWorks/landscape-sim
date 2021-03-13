//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/device.h"

#include <set>
#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/queue.h"
#include "lsim/renderer/vlk/queue_families.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
Device::Device(const VkInstance &instance, const VkSurfaceKHR &surface)
    : required_extentions_({VK_KHR_SWAPCHAIN_EXTENSION_NAME}),
      gpu_(instance, surface), queue_families_(gpu_.Handle(), surface),
      device_(Create(gpu_.Handle(), queue_families_)),
      queues_{Queue(device_, queue_families_.Graphics()),
              Queue(device_, queue_families_.Present())} {
  base::Log::Info("renderer", "device", "created");
}

Device::~Device() {
  base::Log::Info("renderer", "device", "destroying..");
  // Destroys logical device and queues created from logical device
  vkDestroyDevice(device_, nullptr);
}

PhysicalDevice &Device::GPU() { return gpu_; }

VkDevice Device::Handle() { return device_; }

const struct Device::Queues &Device::Queues() const { return queues_; }

const class QueueFamilies &Device::QueueFamilies() const {
  return queue_families_;
}

VkDevice Device::Create(const VkPhysicalDevice &gpu,
                        const class QueueFamilies &queue_families) const {
  // Notify what kind of queues and how many device should create
  // We dont know in advance whether queue family capabilities belong to one
  // family or multiple
  const std::set<QueueFamilies::Index> unique_queue_families = {
      queue_families.Graphics(), queue_families.Present()};
  // Create multiple queues if necessery
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  uint32_t number_of_queues = 1;
  float queue_priority = 1.0F;
  for (auto queue_family : unique_queue_families) {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = number_of_queues;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

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

  VkDevice device = VK_NULL_HANDLE;
  ErrorCheck(vkCreateDevice(gpu, &device_create_info, nullptr, &device));
  return device;
}
} // namespace lsim::renderer::vlk