#ifndef LSIM_APPS_ALPHA_H_
#define LSIM_APPS_ALPHA_H_

#include <lsim/platform/i_application.h>

class Alpha : public lsim::platform::IApplication {
public:
  Alpha(int argc, char *argv[]);
};


/*
#include <string>

// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

#include <lsim/types.h>

class Application {
public:
  Application(int argc, char *argv[]);
  ~Application();
  void Run();

private:
  SDL_Window *window_;
  VkInstance instance_;
  VkSurfaceKHR surface_;
  const std::string name_;
};
*/
#endif