//
// Created by Ivars Rusbergs in 2021
//
// Main context of the renderer.
// Stores renderer state and functions.
#include <vector>

#include <lsim/renderer/vlk//instance.h>

namespace lsim::renderer {
class Renderer {
public:
  Renderer(const std::vector<const char *> &extensions);
private:
  const vlk::Instance instance_;
};
  
}