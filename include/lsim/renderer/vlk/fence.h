//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan fence object.
// Fences are used for GPO-to-CPU synchronization.
#ifndef LSIM_RENDERER_VLK_FENCE_H_
#define LSIM_RENDERER_VLK_FENCE_H_
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class Fence {
public:
  // signaled parameter determines if fence is created in already signaled state
  explicit Fence(VkDevice device, bool signaled = true);
  ~Fence();
  Fence(Fence const &) = delete;
  Fence &operator=(Fence const &) = delete;
  Fence(Fence &&other) noexcept;
  Fence &operator=(Fence &&other) noexcept;
  // Returns Vulkan object handle
  [[nodiscard]] VkFence Handle();
  // Waits for the fence to enter signaled state
  void WaitFor();
  // Sets the state of fence to unsignaled from the host
  void Reset();

private:
  [[nodiscard]] VkFence Create(bool signaled) const;
  void Destroy();
  // Pointer to resource this object is created with
  VkDevice const context_device_;
  VkFence fence_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif