//
// Created by Ivars Rusbergs in 2021
//
#include "instance.h"

#include <stdexcept>

#include "SDL_vulkan.h"

namespace renderer::vlk {
Instance::Instance() : instance_(Create()) {}

Instance::~Instance() {}

VkInstance Instance::Create() const {

  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  // TODO
  app_info.pApplicationName = "Provided from outside";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "LSIM";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info{};
create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
create_info.pApplicationInfo = &app_info;
/*
  // Get WSI extensions from SDL
  unsigned extension_count;
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count, NULL)) {
    throw std::runtime_error(
        "Could not get the number of required instance extensions from SDL.");
  }
  std::vector<const char *> extensions(extension_count);
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count,
                                        extensions.data())) {
    throw std::runtime_error(
        "Could not get the names of required instance extensions from SDL.");
  }
*/
/*
  instInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  instInfo.ppEnabledExtensionNames = extensions.data();
  instInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
  instInfo.ppEnabledLayerNames = layers.data();
*/

  VkInstance instance;
  ErrorCheck(vkCreateInstance(&create_info, nullptr, &instance));
  return instance;
}

}; // namespace renderer::vlk
