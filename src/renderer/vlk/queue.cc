//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/queue.h"

#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "vulkan_shared.h"

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

void Queue::Submit(const std::vector<VkCommandBuffer> &command_buffers,
                   const std::vector<VkSemaphore> &wait_semaphores,
                   const std::vector<VkPipelineStageFlags> &wait_stages,
                   const std::vector<VkSemaphore> &signal_semaphores,
                   const VkFence &fence) const {
    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());
    submit_info.pCommandBuffers = command_buffers.data();
    submit_info.waitSemaphoreCount = static_cast<uint32_t>(wait_semaphores.size());
    submit_info.pWaitSemaphores = wait_semaphores.data();
    submit_info.pWaitDstStageMask = wait_stages.data();
    submit_info.signalSemaphoreCount = static_cast<uint32_t>(signal_semaphores.size());
    submit_info.pSignalSemaphores = signal_semaphores.data();
    ErrorCheck(vkQueueSubmit(queue_, 1, &submit_info, fence));
}

} // namespace lsim::renderer::vlk