//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/renderer.h"

#include <vector>

namespace lsim::renderer {
Renderer::Renderer(const std::vector<const char *> &extensions)
    : instance_(extensions) {}

} // namespace lsim::renderer