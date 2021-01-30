//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/instance.h"

#include <stdexcept>
#include <vector>

#include "vulkan_shared.h"

namespace renderer::vlk {
Instance::Instance(const std::vector<const char *> &extensions)
    : instance_(Create(extensions)) {}

Instance::~Instance() {}

VkInstance Instance::Create(const std::vector<const char *> &extensions) const {

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
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();

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

} // namespace renderer::vlk
