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
// TODO: having to add struct and class in front of variables to not mix with
// getters is probably a problem of the class not having a single responsibility.
class Device {
public:
  // Device queue handles that are retrieved from this device
  // Note that queue handles can match.
  struct Queues {
    const Queue graphics;
    const Queue present;
  };

  Device(const VkInstance &instance, const VkSurfaceKHR &surface);
  ~Device();
  Device(Device const &) = delete;
  Device operator=(Device const &) = delete;
  // Returns reference to Vulkan physical device abstraction
  const PhysicalDevice &GPU() const;
  // Returns reference to Vulkan logical device object
  const VkDevice &Handle() const;
  // Returns containing abstractions
  const struct Queues &Queues() const;
  const class QueueFamilies &QueueFamilies() const;

private:
  // Creates and return handle to logical device from the given gpu
  VkDevice Create(const VkPhysicalDevice &gpu,
                  const class QueueFamilies &queue_families) const;
  // Device extentions that the logical device has to support
  const std::vector<const char *> required_extentions_;
  // The selected device Vulkan is going to use, aka physical device
  const PhysicalDevice gpu_;
  const class QueueFamilies queue_families_;
  const VkDevice device_ = VK_NULL_HANDLE;
  const struct Queues queues_;
};
} // namespace lsim::renderer::vlk

#endif