//
// Created by Ivars Rusbergs in 2021
//
// Validation layers, debug messanger and callback
#ifndef LSIM_RENDERER_VLK_VALIDATION_H_
#define LSIM_RENDERER_VLK_VALIDATION_H_
#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class Validation {
public:
  Validation();
  bool Enabled() const;
  // Add required extentions for our validation layers to the given extentions
  // vector
  void AppendExtentions(std::vector<const char *> &extensions) const;
  const std::vector<const char *> &GetLayers() const;

private:
  // Checks and throws exception if validation layers are not supported
  void CheckSupport() const;
  bool IsLayersSupported() const;
  // Enable validation layers depending on if this is a debug build
#ifdef _DEBUG
  static constexpr bool enabled_ = true;
#else
  static constexpr bool enabled_ = false;
#endif
  // Validation layers
  const std::vector<const char *> layers_;
};
} // namespace lsim::renderer::vlk

#endif