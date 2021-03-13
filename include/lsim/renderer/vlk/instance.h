//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a Vulkan application instance object
// Instance stores all aplication-wide Vulkan state
#ifndef LSIM_RENDERER_VLK_INSTANCE_H_
#define LSIM_RENDERER_VLK_INSTANCE_H_
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

#include "lsim/platform/types.h"
#include "validation.h"

namespace lsim::renderer::vlk {
class Instance {
public:
  Instance(SDL_Window *window, const platform::Settings &settings);
  ~Instance();
  Instance(Instance const &) = delete;
  Instance &operator=(Instance const &) = delete;
  Instance(Instance &&) = delete;
  Instance &operator=(Instance &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkInstance Handle();

private:
  [[nodiscard]] VkInstance Create(const std::string &name, uint32_t version,
                                  SDL_Window *window) const;
  // Validation layers and debug callbacks
  Validation validation_;
  VkInstance const instance_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif