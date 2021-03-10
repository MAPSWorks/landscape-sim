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
  // Alias for extension vector data ttype
  using CharVector = std::vector<const char *>;
  Instance(SDL_Window *window, const platform::Settings &settings);
  ~Instance();
  Instance(Instance const &) = delete;
  Instance &operator=(Instance const &) = delete;
  Instance(Instance &&) = delete;
  Instance &operator=(Instance &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] const VkInstance &Handle() const;
  // Returns if validation layers are enabled.
  // To not expose the whole validation object.
  [[nodiscard]] bool ValidationEnabled() const;

private:
  // Gets array of required extensions and appends other.
  // Returns updated extension vector.
  CharVector GetExtensions(SDL_Window *window) const;
  [[nodiscard]] VkInstance Create(const std::string &name,
                                  uint32_t version) const;
  // Validation layers and debug callbacks
  Validation validation_;
  CharVector extensions_;
   // NOLINTNEXTLINE - const pointer, not const pointee
  VkInstance const instance_ = VK_NULL_HANDLE;
};
} // namespace lsim::renderer::vlk

#endif