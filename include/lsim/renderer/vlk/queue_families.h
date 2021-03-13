//
// Created by Ivars Rusbergs in 2021
//
// Vulkan queue family selection and storage.
#ifndef LSIM_RENDERER_VLK_QUEUE_FAMILIES_H_
#define LSIM_RENDERER_VLK_QUEUE_FAMILIES_H_
#include <optional>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Class is designed for storing and selecting graphics and presentation
// families
class QueueFamilies {
public:
  // Queue family index
  using Index = uint32_t;
  QueueFamilies(VkPhysicalDevice gpu, VkSurfaceKHR surface);
  // Checks if all required queue families are available
  [[nodiscard]] bool Complete() const;
  // Checks if graphics and presentation queues families are the same
  [[nodiscard]] bool GraphicsPresentMatch() const;
  // Accessor
  [[nodiscard]] Index Graphics() const;
  // Accessor
  [[nodiscard]] Index Present() const;

private:
  // Selects queue family indices from the given gpu and surface.
  // Initializes graphhics and present families if found.
  void Select(VkPhysicalDevice gpu, VkSurfaceKHR surface);
  // Index of a graphics queue family
  // It is possible that it is not present.
  std::optional<Index> graphics_;
  // Index of a presentation queue family
  // It is possible that it is not present.
  std::optional<Index> present_;
};
} // namespace lsim::renderer::vlk

#endif