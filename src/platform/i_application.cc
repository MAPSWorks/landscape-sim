//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/platform/i_application.h"

#include <SDL2/SDL.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"

namespace lsim::platform {
IApplication::IApplication(int argc, char *argv[], const Settings &settings)
    : settings_(settings), window_(settings),
      renderer_(window_.Handle(), settings) {
  (void)argc;
  (void)argv;

  base::Log::Info("platform", "initialized");
}

IApplication::~IApplication() {
  base::Log::Info("platform", "shuttind down..");
}

void IApplication::Run() {
  base::Log::Info("platform", "main loop", "entering..");
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
        break;
      }
    }
      
    RenderFrame(); 
    //running = false;
    //SDL_Delay(10);
  }
  base::Log::Info("platform", "main loop", "exiting..");
}

} // namespace lsim::platform