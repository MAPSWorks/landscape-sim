//
// Created by Ivars Rusbergs in 2021
//
// Controls the way validation layer feedback is returned
// Usi this class after application Instance has been created.
#ifndef LSIM_RENDERER_VLK_DEBUG_MESSENGER_H_
#define LSIM_RENDERER_VLK_DEBUG_MESSENGER_H_
#include <vulkan/vulkan.h>

#include "instance.h"

namespace lsim::renderer::vlk {
class DebugMessenger {
public:
  DebugMessenger(const Instance& instance);
  ~DebugMessenger();
  DebugMessenger(DebugMessenger const &) = delete;
  DebugMessenger operator=(DebugMessenger const &) = delete;

private: 
  // Reports back from validation layers
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                VkDebugUtilsMessageTypeFlagsEXT message_type,
                const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                void *user_data);
  void Create();
  // Reference to application instance this messenger is used with
  const Instance& instance_;
  VkDebugUtilsMessengerEXT debug_messanger_ = VK_NULL_HANDLE;
};

} // namespace lsim::renderer::vlk

#endif