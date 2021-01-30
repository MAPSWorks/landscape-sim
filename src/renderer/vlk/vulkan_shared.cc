//
// Created by Ivars Rusbergs in 2021
//
#include "vulkan_shared.h"

#include <stdexcept>
#include <string>

namespace renderer::vlk {
void ErrorCheck(VkResult result) {
  std::string error_str = "Unhandled error";
  switch (result) {
  case VK_ERROR_OUT_OF_HOST_MEMORY:
    error_str = "A host memory allocation has failed.";
    break;
  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    error_str = "A device memory allocation has failed.";
    break;
  case VK_ERROR_INITIALIZATION_FAILED:
    error_str = "Initialization of an object could not be completed for "
                "implementation - specific reasons.";
    break;
  case VK_ERROR_DEVICE_LOST:
    error_str = "The logical or physical device has been lost.See Lost Device";
    break;
  case VK_ERROR_MEMORY_MAP_FAILED:
    error_str = "Mapping of a memory object has failed.";
    break;
  case VK_ERROR_LAYER_NOT_PRESENT:
    error_str = "A requested layer is not present or could not be loaded.";
    break;
  case VK_ERROR_EXTENSION_NOT_PRESENT:
    error_str = "A requested extension is not supported.";
    break;
  case VK_ERROR_FEATURE_NOT_PRESENT:
    error_str = "A requested feature is not supported.";
    break;
  case VK_ERROR_INCOMPATIBLE_DRIVER:
    error_str =
        "The requested version of Vulkan is not supported by the driver or is "
        "otherwise incompatible for implementation - specific reasons.";
    break;
  case VK_ERROR_TOO_MANY_OBJECTS:
    error_str = "Too many objects of the type have already been created.";
    break;
  case VK_ERROR_FORMAT_NOT_SUPPORTED:
    error_str = "A requested format is not supported on this device.";
    break;
  case VK_ERROR_FRAGMENTED_POOL:
    error_str =
        "A pool allocation has failed due to fragmentation of the pool’s "
        "memory.This must only be returned if no attempt to allocate host or "
        "device "
        "memory was made to accommodate the new allocation.This should be "
        "returned in preference to VK_ERROR_OUT_OF_POOL_MEMORY, but only if "
        "the implementation is "
        "certain that the pool allocation failure was due to fragmentation.";
    break;
  case VK_ERROR_SURFACE_LOST_KHR:
    error_str = "A surface is no longer available.";
    break;
  case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
    error_str = "The requested window is already in use by Vulkan or another "
                "API in a manner which prevents it from being used again.";
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    error_str = "A surface has changed in such a way that it is no longer "
                "compatible with the swapchain, and further presentation "
                "requests using the swapchain "
                "will fail.Applications must query the new surface "
                "propertiesand recreate their swapchain if they wish to "
                "continue presenting to the surface.";
    break;
  case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
    error_str = "The display used by a swapchain does not use the same "
                "presentable image layout, or is incompatible in a way that "
                "prevents sharing an image.";
    break;
  case VK_ERROR_INVALID_SHADER_NV:
    error_str = "One or more shaders failed to compile or link.More details "
                "are reported back to the application via "
                "https "
                "://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/"
                "vkspec.html#VK_EXT_debug_report if enabled.";
    break;
  case VK_ERROR_OUT_OF_POOL_MEMORY:
    error_str =
        "A pool memory allocation has failed.This must only be returned if no "
        "attempt to allocate host or device memory was made to accommodate the "
        "new "
        "allocation.If the failure was definitely due to fragmentation of the "
        "pool, VK_ERROR_FRAGMENTED_POOL should be returned instead.";
    break;
  case VK_ERROR_INVALID_EXTERNAL_HANDLE:
    error_str =
        "An external handle is not a valid handle of the specified type.";
    break;
  case VK_ERROR_FRAGMENTATION:
    error_str = "A descriptor pool creation has failed due to fragmentation.";
    break;
  case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
    error_str = "A buffer creation failed because the requested address is not "
                "available.";
    break;
  case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
    error_str = "An operation on a swapchain created with "
                "VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as "
                "it did not have exlusive full - "
                "screen access.This may occur due to implementation - "
                "dependent reasons, outside of the application’s control";
    break;
  case VK_ERROR_UNKNOWN: 
    error_str = "An unknown error has occurred; either the application has "
                "provided invalid input, or an implementation failure has "
                "occurred.";
    break;
    /*
  case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
    error_str = "A buffer creation or memory allocation failed because the"
                " requested address is not available. A shader group handle "
                "assignment failed because the requested shader group handle "
                "information is no longer valid."
    break;
    */
  default:
    break;
  }
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Vulkan: " + error_str);
  }
}
} // namespace renderer::vlk
