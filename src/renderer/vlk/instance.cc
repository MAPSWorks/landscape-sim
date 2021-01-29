//
// Created by Ivars Rusbergs in 2021
//
#include "instance.h"

#include "SDL_vulkan.h"

namespace renderer::vlk {
Instance::Instance() : instance_(Create()) {}

Instance::~Instance() {}

VkInstance Instance::Create() const {

  VkInstanceCreateInfo create_info{};

  VkInstance instance;
  ErrorCheck(vkCreateInstance(&create_info, nullptr, &instance));
  return instance;
}

}; // namespace renderer::vlk
