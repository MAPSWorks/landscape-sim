//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/queue.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"

namespace lsim::renderer::vlk {
// Internal linkage
namespace {
// Retrieve queue from the given device
VkQueue GetDeviceQueue(const VkDevice &device, uint32_t family_index) {
  VkQueue queue;
  vkGetDeviceQueue(device, family_index, 0, &queue);
  return queue;
}

} // namespace

Queue::Queue(const VkDevice &device, uint32_t family_index)
    : queue_(GetDeviceQueue(device, family_index)) {

  base::Log::Info("renderer", "queue", "retrieved", "with family index",
                  family_index);
}

const VkQueue &Queue::Handle() const { return queue_; }

} // namespace lsim::renderer::vlk