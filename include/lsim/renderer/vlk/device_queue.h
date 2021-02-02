//
// Created by Ivars Rusbergs in 2021
//
#ifndef LSIM_RENDERER_VLK_DEVICE_QUEUE_H_
#define LSIM_RENDERER_VLK_DEVICE_QUEUE_H_
#include <optional>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
using QueueFamilyIndex = uint32_t;
class DeviceQueue {
public:
  // Required device queue family types and their existance
  struct FamilyIndices {
    std::optional<QueueFamilyIndex> graphics;
    // std::optional<QueueFamilyIndex> present;
    bool IsComplete() const {
      return graphics.has_value() /*&& present.has_value()*/;
    }
    // If grphics and present is the same queueu
    /*
    bool IsGraphicsPresentMatch() const {
      return graphics.value() == present.value();
    }*/
  };
  DeviceQueue(const VkPhysicalDevice &gpu);

private:
  // Finds suitable queue families and store their indices
  FamilyIndices SelectFamilies(const VkPhysicalDevice &gpu) const;
  // Selected queue family indices
  const FamilyIndices family_indices_;
};
} // namespace lsim::renderer::vlk
#endif