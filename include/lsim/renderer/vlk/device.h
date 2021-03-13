//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan lofical device object.
// Is a container for physical device, queue family indexes and queues.
#ifndef LSIM_RENDERER_VLK_DEVICE_H_
#define LSIM_RENDERER_VLK_DEVICE_H_
#include <vector>

#include <vulkan/vulkan.h>

#include "physical_device.h"
#include "queue.h"
#include "queue_families.h"

namespace lsim::renderer::vlk {
// TODO(ivars): having to add struct and class in front of variables to not mix
// with getters is probably a problem of the class not having a single
// responsibility.
class Device {
public:
  // Device queue handles that are retrieved from this device
  // Note that queue handles can match.
  struct Queues {
    const Queue graphics;
    const Queue present;
  };

  Device(VkInstance instance, VkSurfaceKHR surface);
  ~Device();
  Device(Device const &) = delete;
  Device &operator=(Device const &) = delete;
  Device(Device &&) = delete;
  Device &operator=(Device &&) = delete;
  // Returns reference to Vulkan physical device abstraction
  [[nodiscard]] PhysicalDevice &GPU();
  // Returns reference to Vulkan logical device object
  [[nodiscard]] VkDevice Handle();
  // Returns containing abstractions
  [[nodiscard]] const struct Queues &Queues() const;
  [[nodiscard]] const class QueueFamilies &QueueFamilies() const;

private:
  // Creates and return handle to logical device from the given gpu
  [[nodiscard]] VkDevice
  Create(VkPhysicalDevice gpu,
         const class QueueFamilies &queue_families) const;
  // Device extentions that the logical device has to support
  const std::vector<const char *> required_extentions_;
  // The selected device Vulkan is going to use, aka physical device
  PhysicalDevice gpu_;
  const class QueueFamilies queue_families_;
  VkDevice const device_ = VK_NULL_HANDLE;
  const struct Queues queues_;
};
} // namespace lsim::renderer::vlk

#endif