//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/validation.h"

#include <cstring>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
Validation::Validation() {
  if (enabled_) {
    // Prior to Vulkan SDK for 1.1.106.0 layers:
    // VK_LAYER_LUNARG_standard_validation Starting from Vulkan SDK
    // for 1.1.106.0 : VK_LAYER_KHRONOS_validation
    layers_.push_back("VK_LAYER_KHRONOS_validation");
    // base::Log::Info("Renderer: validation enabled");
    if (!IsLayersSupported()) {
      throw std::runtime_error(
          "Renderer: requested validation layers are not available!");
    }
  }
}

void Validation::AppendExtentions(std::vector<const char *> &extensions) const {
  if (enabled_) {
    // Same as 'VK_EXT_debug_report'
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
}

bool Validation::Enabled() const { return enabled_; }

const std::vector<const char *> &Validation::GetLayers() const {
  return layers_;
}

bool Validation::IsLayersSupported() const {
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
  for (auto name_ : layers_) {
    bool layer_found_ = false;
    for (const auto &properties_ : available_layers) {
      if (strcmp(name_, properties_.layerName) == 0) {
        layer_found_ = true;
        break;
      }
    }
    if (!layer_found_) {
      return false;
    }
  }

  return true;
}
} // namespace lsim::renderer::vlk