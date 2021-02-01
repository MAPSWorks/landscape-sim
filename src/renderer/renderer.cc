//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/renderer.h"

#include <vector>

#include "lsim/base/log.h"

namespace lsim::renderer {
Renderer::Renderer(const std::vector<const char *> &extensions)
    : instance_(extensions), debug_messenger_(instance_) {
  base::Log::Info("renderer", "initialized");
}

} // namespace lsim::renderer