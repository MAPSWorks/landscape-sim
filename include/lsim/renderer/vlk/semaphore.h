//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan semaphore object.
// Semaphores are used to synchronize operations within or across command
// queues.
#ifndef LSIM_RENDERER_VLK_SEMAPHORE_H_
#define LSIM_RENDERER_VLK_SEMAPHORE_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class Semaphore {
public:
  Semaphore(const VkDevice &device);
  ~Semaphore();
  Semaphore(Semaphore const &) = delete;
  Semaphore operator=(Semaphore const &) = delete;
  // Returns Vulkan object handle
  const VkSemaphore &Handle() const;

private:
  VkSemaphore Create() const;
  // Reference to resource this object is created with
  const VkDevice &device_;
  const VkSemaphore semaphore_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif