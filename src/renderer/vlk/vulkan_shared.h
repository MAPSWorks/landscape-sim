#pragma once
#include <string>
#include <stdexcept>
#include <vulkan/vulkan.h>
//#include <vulkan/vulkan.hpp>
// Vulkan helper functions
namespace renderer::vlk {
// Throws if return code equals error code
void ErrorCheck(VkResult result);
// For debugging, textual values of parameter flags
std::string ToString(VkShaderStageFlagBits value);
std::string ToString(VkPrimitiveTopology value);
std::string ToString(VkPolygonMode value);
std::string ToString(VkCullModeFlagBits value);
std::string ToString(bool value);
// Load vulkan extention functions
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, 
    VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator);
}; // renderer vlk
