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
  explicit Window(const Settings &settings);
  ~Window();
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(Window &&) = delete;
  // Get window handle
  [[nodiscard]] SDL_Window *Handle() const;

private:
  // OS window handle, used to retreive data like size etc
  SDL_Window *window_;
};
} // namespace lsim::platform

#endif