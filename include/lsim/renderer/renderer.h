//
// Created by Ivars Rusbergs in 2021
//
// Main context of the renderer.
// Stores renderer state and functions.
#include <vector>

#include <SDL2/SDL.h>

#include "lsim/platform/types.h"
#include "vlk/debug_messenger.h"
#include "vlk/device.h"
#include "vlk/instance.h"

namespace lsim::renderer {
class Renderer {
public:
  Renderer(SDL_Window *window, const platform::Settings &settings);

private:
  const vlk::Instance instance_;
  // Responsible for validation info feedback output.
  // Dependent on Instance being already initialized.
  const vlk::DebugMessenger debug_messenger_;
  const vlk::Device device_;
};

} // namespace lsim::renderer