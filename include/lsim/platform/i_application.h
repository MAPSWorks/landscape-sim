//
// Created by Ivars Rusbergs in 2021
//
#ifndef LSIM_PLATFORM_IAPPLICATION_H_
#define LSIM_PLATFORM_IAPPLICATION_H_

#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "types.h"

namespace lsim::platform {
// Abstract base class to be used as a parent class for apps.
// Handles application specific tasks and stores it's state.
class IApplication {
public:
  IApplication(int argc, char *argv[], const Settings &settings);
  virtual ~IApplication();
  IApplication(IApplication const &) = delete;
  // Copy-assignment is not allowed doe to pure virtual functions
  // Main loop of an Application
  void Run();

protected:
  // Applications and subsystem settings
  const Settings settings_;
  // OS window handle, used to retreive data like size etc
  SDL_Window *window_;
private:
  SDL_Window *CreatWindow() const;
};
} // namespace lsim::platform

#endif