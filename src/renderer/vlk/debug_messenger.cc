//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/debug_messenger.h"

#include <iostream>

#include <vulkan/vulkan.h>

#include "lsim/renderer/vlk/instance.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
DebugMessenger::DebugMessenger(const Instance &instance)
    : instance_(instance), debug_messanger_(Create()) {}

DebugMessenger::~DebugMessenger() {
  if (instance_.ValidationEnabled()) {
    DestroyDebugUtilsMessengerEXT(instance_.Get(), debug_messanger_, nullptr);
  }
}

VkDebugUtilsMessengerEXT DebugMessenger::Create() const {
  if (instance_.ValidationEnabled()) {
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = DebugCallback;
    create_info.pUserData =
        nullptr; // Optional (goes to user_data in debug callback)
    VkDebugUtilsMessengerEXT debug_messenger;
    ErrorCheck(CreateDebugUtilsMessengerEXT(instance_.Get(), &create_info,
                                            nullptr, &debug_messenger));
    return debug_messenger;
  } else {
    return VK_NULL_HANDLE;
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT /*message_severity*/,
    VkDebugUtilsMessageTypeFlagsEXT /*message_type*/,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void * /*user_data*/) {
  // base::Log::Error("* Validation layer: ", callback_data->pMessage);
  std::cout << "*** Validation layer: " << callback_data->pMessage << std::endl;
  return VK_FALSE;
}

} // namespace lsim::renderer::vlk
