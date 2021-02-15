//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/queue.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"

namespace lsim::renderer::vlk {
Queue::Queue(const VkDevice &device, uint32_t family_index)
    : queue_(GetFromDevice(device, family_index)) {

  base::Log::Info("renderer", "queue", "retrieved", "with family index",
                  family_index);
}

const VkQueue &Queue::Handle() const { return queue_; }

VkQueue Queue::GetFromDevice(const VkDevice &device,
                             uint32_t family_index) const {
  VkQueue queue;
  vkGetDeviceQueue(device, family_index, 0, &queue);
  return queue;
}

} // namespace lsim::renderer::vlk