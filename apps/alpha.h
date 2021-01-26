#ifndef LSIM_APPS_ALPHA_H_
#define LSIM_APPS_ALPHA_H_
// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
//#define GLM_FORCE_MESSAGES
//#include <glm/glm.hpp>

class Application {
public:
  Application(int argc, char *argv[]);
  ~Application();
  void Run();

private:
  SDL_Window *window_;
  VkInstance instance_;
  VkSurfaceKHR surface_;
};

#endif