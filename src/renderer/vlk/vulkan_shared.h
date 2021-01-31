//
// Created by Ivars Rusbergs in 2021
//
// Commonly used Vulkan types and helper functions
#ifndef LSIM_RENDERER_VLK_VULKAN_SHARED_H_
#define LSIM_RENDERER_VLK_VULKAN_SHARED_H_

#include <vulkan/vulkan.h>

namespace lsim::renderer::vlk {
// Checks given result and throws exception if it is an error
void ErrorCheck(VkResult result);

} // namespace renderer::vlk

#endif