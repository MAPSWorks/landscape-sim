//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan device queue object.
#ifndef LSIM_RENDERER_VLK_QUEUE_H_
#define LSIM_RENDERER_VLK_QUEUE_H_
#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Queues are created together with logial device.
// Queues are cleaned up when the logical device is destroyed.
// This class is only a storage of handle and associated functions.
class Queue {
public:
  Queue(const VkDevice &device, uint32_t family_index);
  [[nodiscard]] VkQueue Handle();
  void Submit(const std::vector<VkCommandBuffer> &command_buffers,
              const std::vector<VkSemaphore> &wait_semaphores,
              const std::vector<VkPipelineStageFlags> &wait_stages,
              const std::vector<VkSemaphore> &signal_semaphores,
              const VkFence &fence) const;
  // Present image to swapchain
  // Calling queue have to be with pesentation capabilities
  // wait_semaphore is semaphore to wait on before presenting
  [[nodiscard]] VkResult Present(const VkSwapchainKHR &swapchain,
                                 uint32_t image_index,
                                 const VkSemaphore &wait_semaphore) const;

private:
  VkQueue const queue_ = VK_NULL_HANDLE;
};

} // namespace lsim::renderer::vlk

#endif