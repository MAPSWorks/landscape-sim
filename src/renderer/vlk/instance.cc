//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/instance.h"

#include <stdexcept>
#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"
#include "lsim/renderer/vlk/validation.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
Instance::Instance(ExtVector extensions, const platform::Settings &settings)
    : extensions_(AppendExtensions(extensions)),
      instance_(Create(extensions_, settings.name, settings.version)) {
  base::Log::Info("renderer", "instance", "created");
}

Instance::~Instance() {
  base::Log::Info("renderer", "instance", "destroying..");
  vkDestroyInstance(instance_, nullptr);
}

const VkInstance &Instance::Get() const { return instance_; }

VkInstance Instance::Create(const ExtVector &extensions, std::string name,
                            uint32_t version) const {
  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  // TODO
  app_info.pApplicationName = name.c_str();
  app_info.applicationVersion = version;
  app_info.pEngineName = "LSIM";
  app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  // Extensions
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();
  // Validation layers
  create_info.enabledLayerCount =
      static_cast<uint32_t>(validation_.GetLayers().size());
  create_info.ppEnabledLayerNames = validation_.GetLayers().data();

  base::Log::Info("renderer", "app name:", app_info.pApplicationName,
                  " app version:", app_info.applicationVersion,
                  " engine name:", app_info.pEngineName,
                  " engine version:", app_info.engineVersion);
  VkInstance instance;
  ErrorCheck(vkCreateInstance(&create_info, nullptr, &instance));
  return instance;
}

Instance::ExtVector Instance::AppendExtensions(ExtVector extensions) const {
  validation_.AppendExtentions(extensions);
  return extensions;
}

bool Instance::ValidationEnabled() const { return validation_.Enabled(); }

} // namespace lsim::renderer::vlk
