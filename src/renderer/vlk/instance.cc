//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/instance.h"
#include "SDL_stdinc.h"

#include <stdexcept>
#include <string>
#include <vector>

// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"
#include "lsim/renderer/vlk/validation.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
namespace {
// Retrieves and return a vector of required instance extensions
// This function is platform dependant
std::vector<const char *> GetExtensions(SDL_Window *window) {
  // Get WSI extensions from SDL
  unsigned extension_count = 0;
  if (SDL_Vulkan_GetInstanceExtensions(window, &extension_count, nullptr) ==
      SDL_FALSE) {
    throw std::runtime_error("renderer: could not get the number of required "
                             "instance extensions from SDL.");
  }
  std::vector<const char *> extensions(extension_count);
  if (SDL_Vulkan_GetInstanceExtensions(window, &extension_count,
                                       extensions.data()) == SDL_FALSE) {
    throw std::runtime_error("renderer: could not get the names of required "
                             "instance extensions from SDL.");
  }
  return extensions;
}
} // namespace

Instance::Instance(SDL_Window *window, const platform::Settings &settings)
    : instance_(Create(settings.name, settings.version, window)) {
  base::Log::Info("renderer", "instance", "created");
}

Instance::~Instance() {
  base::Log::Info("renderer", "instance", "destroying..");
  vkDestroyInstance(instance_, nullptr);
}

VkInstance Instance::Handle() const { return instance_; }

VkInstance Instance::Create(const std::string &name, uint32_t version,
                            SDL_Window *window) const {
  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = name.c_str();
  app_info.applicationVersion = version;
  app_info.pEngineName = "LSIM";
  app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
  app_info.apiVersion = VK_API_VERSION_1_1;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  // Extensions
  auto extensions = GetExtensions(window);
  // Add extentions from validation
  validation_.AppendExtentions(extensions);
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();
  // Validation layers
  create_info.enabledLayerCount =
      static_cast<uint32_t>(validation_.Layers().size());
  create_info.ppEnabledLayerNames = validation_.Layers().data();

  base::Log::Info("renderer", "app name:", app_info.pApplicationName,
                  " app version:", app_info.applicationVersion,
                  " engine name:", app_info.pEngineName,
                  " engine version:", app_info.engineVersion);
  VkInstance instance = VK_NULL_HANDLE;
  ErrorCheck(vkCreateInstance(&create_info, nullptr, &instance));
  return instance;
}

bool Instance::ValidationEnabled() const { return validation_.Enabled(); }

} // namespace lsim::renderer::vlk
