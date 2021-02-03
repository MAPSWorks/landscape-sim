//
// Created by Ivars Rusbergs in 2021
//
// Queues are not created explicitly and are not created in this class.
// They are created together with and owned by logical device.
// This class selects queue family indices and provides queue creation structure
// to logical device.
// It also stores queue handles that are retrieved from logical device AFTER
// creation of this class. Order is this: select families - create logical
// device - retrieve queue handles
#ifndef LSIM_RENDERER_VLK_DEVICE_QUEUE_H_
#define LSIM_RENDERER_VLK_DEVICE_QUEUE_H_
#include <optional>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
using QueueFamilyIndex = uint32_t;
// Physical device should be selected before using this class
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
  // Finds suitable queue families and store their indices
  static FamilyIndices SelectFamilies(const VkPhysicalDevice &gpu);

private:
  // Selected queue family indices
  const FamilyIndices family_indices_;
};
} // namespace lsim::renderer::vlk
#endif