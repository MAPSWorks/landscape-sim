//
// Created by Ivars Rusbergs in 2021
//
// Main context of the renderer.
// Stores renderer state and functions.
#include <vector>

#include "vlk/debug_messenger.h"
#include "vlk/instance.h"

namespace lsim::renderer {
class Renderer {
public:
  Renderer(const std::vector<const char *> &extensions);

private:
  const vlk::Instance instance_;
  // Responsible for validation info feedback output.
  // Dependent on Instance being already initialized.
  const vlk::DebugMessenger debug_messenger_;
};

} // namespace lsim::renderer