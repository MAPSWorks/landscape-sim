//
// Created by Ivars Rusbergs in 2021
//
#ifndef LSIM_PLATFORM_IAPPLICATION_H_
#define LSIM_PLATFORM_IAPPLICATION_H_

#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include <lsim/renderer/renderer.h>

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
  // Extensions retrieved from OS and used for instance creation
  const std::vector<const char *> instance_extensions_;
  // Engine renderer instance
  //renderer::Renderer renderer_;

private:
  SDL_Window *CreatWindow() const;
  std::vector<const char *> RetrieveExtensions() const;
};
} // namespace lsim::platform

#endif