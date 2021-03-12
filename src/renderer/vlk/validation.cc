//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/validation.h"

#include <cstring>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"

namespace lsim::renderer::vlk {

// static
void Validation::AppendExtentions(std::vector<const char *> &extensions) {
  if (kEnabled) {
    // Same as 'VK_EXT_debug_report'
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
}

Validation::Validation() {
  if (kEnabled) {
    base::Log::Info("renderer", "validation", "enabled");
    // Prior to Vulkan SDK for 1.1.106.0 layers:
    // VK_LAYER_LUNARG_standard_validation Starting from Vulkan SDK
    // for 1.1.106.0 : VK_LAYER_KHRONOS_validation
    layers_.push_back("VK_LAYER_KHRONOS_validation");
    // base::Log::Info("Renderer: validation enabled");
    if (!IsLayersSupported()) {
      throw std::runtime_error(
          "renderer: requested validation layers are not available!");
    }
  }
}

const std::vector<const char *> &Validation::Layers() const { return layers_; }

bool Validation::IsLayersSupported() const {
  uint32_t layer_count = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
  for (const auto *name : layers_) {
    bool layer_found = false;
    for (const auto &properties : available_layers) {
      if (strcmp(name, properties.layerName) == 0) {
        layer_found = true;
        break;
      }
    }
    if (!layer_found) {
      return false;
    }
  }

  return true;
}
} // namespace lsim::renderer::vlk