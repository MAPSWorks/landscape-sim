//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/renderer.h"

#include <vector>

#include <SDL2/SDL.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"
#include "lsim/renderer/vlk/swapchain.h"

namespace lsim::renderer {
Renderer::Renderer(SDL_Window *window, const platform::Settings &settings)
    : instance_(window, settings), debug_messenger_(instance_),
      surface_(instance_.Get(), window),
      device_(instance_.Get(), surface_.Get()),
      swapchain_(device_.Get(), device_.GetGPU(), surface_.Get()) {
  base::Log::Info("renderer", "initialized");
}

} // namespace lsim::renderer