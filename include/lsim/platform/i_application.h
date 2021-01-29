//
// Created by Ivars Rusbergs in 2021
//
#ifndef LSIM_PLATFORM_IAPPLICATION_H_
#define LSIM_PLATFORM_IAPPLICATION_H_

#include <string>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace lsim::platform {
// Abstract base class to be used as a parent class for apps.
// Handles application specific tasks and stores it's state.
class IApplication {
public:
  // Parameters similar to main() function
  IApplication(int argc, char *argv[]);
  virtual ~IApplication();
  IApplication(IApplication const &) = delete;
  // Copy-assignment is not allowed doe to pure virtual functions
  // Main loop of an Application
  void Run();

protected:
  // Application name
  const std::string name_ = "not set";
  // OS window handle, used to retreive data like size etc
  SDL_Window *window_;
private:

};
} // namespace lsim::platform

#endif