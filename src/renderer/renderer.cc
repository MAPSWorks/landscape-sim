//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/renderer.h"

#include <vector>

#include <SDL2/SDL.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"

namespace lsim::renderer {
Renderer::Renderer(SDL_Window *window, const platform::Settings &settings)
    : instance_(window, settings), debug_messenger_(instance_),
      device_(instance_.Get()) {
  base::Log::Info("renderer", "initialized");
}

} // namespace lsim::renderer