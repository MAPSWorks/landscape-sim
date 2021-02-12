//
// Created by Ivars Rusbergs in 2021
//
// Abstraction of a pltaform-dependant window
#ifndef LSIM_PLATFORM_WINDOW_H_
#define LSIM_PLATFORM_WINDOW_H_

#include <SDL2/SDL.h>

#include "types.h"

namespace lsim::platform {
class Window {
public:
  Window(const Settings &settings);
  ~Window();
  Window(Window const &) = delete;
  Window operator=(Window const &) = delete;
  // Get window handle
  SDL_Window *Handle() const;

private:
  SDL_Window *CreatWindow(const Settings &settings) const;
  // OS window handle, used to retreive data like size etc
  SDL_Window *window_;
};
} // namespace lsim::platform

#endif