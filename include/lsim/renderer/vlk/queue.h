//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan device queue object.
#ifndef LSIM_RENDERER_VLK_QUEUE_H_
#define LSIM_RENDERER_VLK_QUEUE_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Queues are created together with logial device.
// Queues are cleaned up when the logical device is destroyed.
// This class is only a storage of handle and associated functions.
class Queue {
public:
  Queue(const VkDevice& device, uint32_t family_index);
  const VkQueue& Handle() const;
private:
  // Retrieve queue from the given device 
  VkQueue GetFromDevice(const VkDevice& device, uint32_t family_index) const; 
  const VkQueue queue_ = VK_NULL_HANDLE;
};

} // namespace lsim::renderer::vlk

#endif