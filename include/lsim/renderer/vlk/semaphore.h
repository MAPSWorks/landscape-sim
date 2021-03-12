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
  explicit Semaphore(VkDevice device);
  ~Semaphore();
  Semaphore(Semaphore const &) = delete;
  Semaphore& operator=(Semaphore const &) = delete;
  Semaphore(Semaphore &&) = delete;
  Semaphore &operator=(Semaphore &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkSemaphore Handle() const;

private:
  [[nodiscard]] VkSemaphore Create() const;
  // Pointer to resource this object is created with
  VkDevice const context_device_;
  VkSemaphore const semaphore_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif