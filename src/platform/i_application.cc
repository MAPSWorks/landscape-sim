//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/platform/i_application.h"

#include <stdexcept>
#include <vector>

#include <SDL2/SDL.h>
//#include <SDL2/SDL_syswm.h>
//#include <SDL2/SDL_vulkan.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"

namespace lsim::platform {
IApplication::IApplication(int argc, char *argv[], const Settings &settings)
    : settings_(settings), window_(CreatWindow()) {
  (void)argc;
  (void)argv;

  base::Log::Info("platform", "initialized");
}

IApplication::~IApplication() {
  base::Log::Info("platform", "shuttind down..");
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void IApplication::Run() {
  base::Log::Info("platform", "entering main loop");
  // Poll for user input.
  bool running = true;
  while (running) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {

      switch (event.type) {

      case SDL_QUIT:
        running = false;
        break;

      default:
        // Do nothing.
        break;
      }
    }

    // ... render
    running = false;
    SDL_Delay(10);
  }
}

SDL_Window *IApplication::CreatWindow() const {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("platform: could not initialize SDL.");
  }
  SDL_Window *window =
      SDL_CreateWindow(settings_.name.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_VULKAN);
  if (window_ == NULL) {
    throw std::runtime_error("platform: could not create SDL window.");
  }

  return window;
}

} // namespace lsim::platform