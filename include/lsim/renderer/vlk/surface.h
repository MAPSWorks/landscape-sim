//
// Created by Ivars Rusbergs in 2021
//
// OS dependant surface images are presented to
#ifndef LSIM_RENDERER_VLK_SURFACE_H_
#define LSIM_RENDERER_VLK_SURFACE_H_

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
class Surface {
public:
  // Surface depends on OS specific window
  Surface(VkInstance instance, SDL_Window *window);
  ~Surface();
  Surface(Surface const &) = delete;
  Surface &operator=(Surface const &) = delete;
  Surface(Surface &&) = delete;
  Surface &operator=(Surface &&) = delete;
  // Returns Vulkan object handle
  [[nodiscard]] VkSurfaceKHR Handle() const;

private:
  VkSurfaceKHR Create(SDL_Window *window) const;
  // Pointer to resource this renderable was created with.
  VkInstance const context_instance_;
  // Images are presented to the surface
  VkSurfaceKHR const surface_ = VK_NULL_HANDLE;
};

} // namespace lsim::renderer::vlk

#endif