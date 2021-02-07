//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/platform/window.h"

#include <stdexcept>

#include <SDL2/SDL.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"

namespace lsim::platform {
Window::Window(const Settings &settings) : window_(CreatWindow(settings)) {
  base::Log::Info("platform", "window", "initialized",
                  "size:", settings.initial_size.width,
                  settings.initial_size.height);
}

Window::~Window() {
  base::Log::Info("platform", "window", "destroying..");
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

SDL_Window *Window::Get() const { return window_; }

SDL_Window *Window::CreatWindow(const Settings &settings) const {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("platform: could not initialize SDL.");
  }
  SDL_Window *window =
      SDL_CreateWindow(settings.name.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, settings.initial_size.width,
                       settings.initial_size.height, SDL_WINDOW_VULKAN);
  if (window_ == NULL) {
    throw std::runtime_error("platform: could not create SDL window.");
  }

  return window;
}
} // namespace lsim::platform