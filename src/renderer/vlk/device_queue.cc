//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/device_queue.h"

#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {

DeviceQueue::DeviceQueue(const VkPhysicalDevice &gpu)
    : family_indices_(SelectFamilies(gpu)) {}

DeviceQueue::FamilyIndices
DeviceQueue::SelectFamilies(const VkPhysicalDevice &gpu) const {
  // Get all available queue families from given physical device
  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, nullptr);
  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count,
                                           queue_families.data());
  DeviceQueue::FamilyIndices indices;

  uint32_t i = 0;
  for (const auto &family : queue_families) {
    if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics = i;
    }
    /*
    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &present_support);
    if (family.queueCount > 0 && present_support) {
      indices.present = i;
    }
    if (indices.IsComplete()) {
      break;
    }*/

    i++;
  }
  if (!indices.IsComplete()) {
    throw std::runtime_error(
        "renderer: failed to find required device queue families");
  }
  return indices;
}
} // namespace lsim::renderer::vlk