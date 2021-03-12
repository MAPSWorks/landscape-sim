//
// Created by Ivars Rusbergs in 2021
//
#ifndef LSIM_RENDERER_VLK_VALIDATION_H_
#define LSIM_RENDERER_VLK_VALIDATION_H_
#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Validation layers setup and support check.
// Layer validation should be called before instance creation.
class Validation {
public:
  // Add required extentions for our validation layers to the given extentions
  // vector
  static void AppendExtentions(std::vector<const char *> &extensions);
  // Enable validation layers depending on if this is a debug build
#ifdef _DEBUG
  static constexpr bool kEnabled = true;
#else
  static constexpr bool kEnabled = false;
#endif
  Validation();
  [[nodiscard]] const std::vector<const char *> &Layers() const;

private:
  // Check if required layers are available
  [[nodiscard]] bool IsLayersSupported() const;
  // Validation layers
  std::vector<const char *> layers_;
};
} // namespace lsim::renderer::vlk

#endif