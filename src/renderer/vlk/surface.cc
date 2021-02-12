//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/surface.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "lsim/base/log.h"

namespace lsim::renderer::vlk {
Surface::Surface(const VkInstance &instance, SDL_Window *window)
    : instance_(instance), surface_(Create(window)) {
  base::Log::Info("renderer", "surface", "created");
}

Surface::~Surface() {
  base::Log::Info("renderer", "surface", "destroying..");
  vkDestroySurfaceKHR(instance_, surface_, nullptr);
}

const VkSurfaceKHR &Surface::Handle() const { return surface_; }

VkSurfaceKHR Surface::Create(SDL_Window *window) const {
  VkSurfaceKHR surface;
  // No typical error check here because this is SDL function
  if (!SDL_Vulkan_CreateSurface(window, instance_, &surface)) {
    throw std::runtime_error("renderer: failed to create window surface");
  }
  return surface;
}

} // namespace lsim::renderer::vlk