//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/platform/i_application.h"

#include <stdexcept>

// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_vulkan.h>

namespace lsim::platform {
IApplication::IApplication(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Could not initialize SDL.");
  }
    window_ =
      SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_VULKAN);
  if (window_ == NULL) {
    throw std::runtime_error("Could not create SDL window.");
  }
}

IApplication::~IApplication() {

}


void IApplication::Run() {

}

} // namespace lsim::platform