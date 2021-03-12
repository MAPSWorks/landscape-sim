//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/queue_families.h"

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
QueueFamilies::QueueFamilies(const VkPhysicalDevice &gpu,
                             const VkSurfaceKHR &surface) {
  Select(gpu, surface);
}

bool QueueFamilies::Complete() const {
  return graphics_.has_value() && present_.has_value();
}

bool QueueFamilies::GraphicsPresentMatch() const {
  return graphics_.value() == present_.value();
}

QueueFamilies::Index QueueFamilies::Graphics() const {
  return graphics_.value();
}

QueueFamilies::Index QueueFamilies::Present() const { return present_.value(); }

void QueueFamilies::Select(const VkPhysicalDevice &gpu,
                           const VkSurfaceKHR &surface) {
  // Get all available queue families from given physical device
  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, nullptr);
  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count,
                                           queue_families.data());

  uint32_t i = 0;
  // Select only those families that support engine requiements
  for (const auto &family : queue_families) {
    if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      graphics_ = i;
    }

    VkBool32 present_support = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &present_support);
    if (family.queueCount > 0 && present_support == VK_TRUE) {
      present_ = i;
    }
    if (Complete()) {
      // Settling for the selected families
      break;
    }

    ++i;
  }
}

} // namespace lsim::renderer::vlk