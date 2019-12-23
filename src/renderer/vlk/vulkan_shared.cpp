#include "vulkan_shared.h"

namespace renderer::vlk {
void ErrorCheck(VkResult result) {
    std::string error_str;
    switch (result) {
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        error_str = "A host memory allocation has failed.";
        break;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        error_str = "A device memory allocation has failed.";
        break;
    case VK_ERROR_INITIALIZATION_FAILED:
        error_str = "Initialization of an object could not be completed for implementation - specific reasons.";
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
        error_str = "The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation - specific reasons.";
        break;
    case VK_ERROR_TOO_MANY_OBJECTS:
        error_str = "Too many objects of the type have already been created.";
        break;
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        error_str = "A requested format is not supported on this device.";
        break;
    case VK_ERROR_FRAGMENTED_POOL:
        error_str = "A pool allocation has failed due to fragmentation of the pool’s memory.This must only be returned if no attempt to allocate host or device "
            "memory was made to accommodate the new allocation.This should be returned in preference to VK_ERROR_OUT_OF_POOL_MEMORY, but only if the implementation is "
            "certain that the pool allocation failure was due to fragmentation.";
        break;
    case VK_ERROR_SURFACE_LOST_KHR:
        error_str = "A surface is no longer available.";
        break;
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        error_str = "The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again.";
        break;
    case VK_ERROR_OUT_OF_DATE_KHR:
        error_str = "A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain "
            "will fail.Applications must query the new surface propertiesand recreate their swapchain if they wish to continue presenting to the surface.";
        break;
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        error_str = "The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image.";
        break;
    case VK_ERROR_INVALID_SHADER_NV:
        error_str = "One or more shaders failed to compile or link.More details are reported back to the application via "
            "https ://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VK_EXT_debug_report if enabled.";
        break;
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        error_str = "A pool memory allocation has failed.This must only be returned if no attempt to allocate host or device memory was made to accommodate the new "
            "allocation.If the failure was definitely due to fragmentation of the pool, VK_ERROR_FRAGMENTED_POOL should be returned instead.";
        break;
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        error_str = "An external handle is not a valid handle of the specified type.";
        break;
    case VK_ERROR_FRAGMENTATION_EXT:
        error_str = "A descriptor pool creation has failed due to fragmentation.";
        break;
    case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
        error_str = "A buffer creation failed because the requested address is not available.";
        break;
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        error_str = "An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as it did not have exlusive full - "
            "screen access.This may occur due to implementation - dependent reasons, outside of the application’s control";
        break;
    default:
        break;
    }
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Vulkan: " + error_str);
    }
}

std::string ToString(VkShaderStageFlagBits value) {
    switch (value) {
    case VK_SHADER_STAGE_VERTEX_BIT: return "Vertex";
    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return "TessellationControl";
    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return "TessellationEvaluation";
    case VK_SHADER_STAGE_GEOMETRY_BIT: return "Geometry";
    case VK_SHADER_STAGE_FRAGMENT_BIT: return "Fragment";
    case VK_SHADER_STAGE_COMPUTE_BIT: return "Compute";
    case VK_SHADER_STAGE_ALL_GRAPHICS: return "AllGraphics";
    case VK_SHADER_STAGE_ALL: return "All";
    case VK_SHADER_STAGE_RAYGEN_BIT_NV: return "RaygenNV";
    case VK_SHADER_STAGE_ANY_HIT_BIT_NV: return "AnyHitNV";
    case VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV: return "ClosestHitNV";
    case VK_SHADER_STAGE_MISS_BIT_NV: return "MissNV";
    case VK_SHADER_STAGE_INTERSECTION_BIT_NV: return "IntersectionNV";
    case VK_SHADER_STAGE_CALLABLE_BIT_NV: return "CallableNV";
    case VK_SHADER_STAGE_TASK_BIT_NV: return "TaskNV";
    case VK_SHADER_STAGE_MESH_BIT_NV: return "MeshNV";
    default: return "invalid";
    }
}

std::string ToString(VkPrimitiveTopology value) {
    switch (value) {
    case VK_PRIMITIVE_TOPOLOGY_POINT_LIST: return "PointList";
    case VK_PRIMITIVE_TOPOLOGY_LINE_LIST: return "LineList";
    case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: return "LineStrip";
    case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: return "TriangleList";
    case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: return "TriangleStrip";
    case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN: return "TriangleFan";
    case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY: return "LineListWithAdjacency";
    case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY: return "LineStripWithAdjacency";
    case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY: return "TriangleListWithAdjacency";
    case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY: return "TriangleStripWithAdjacency";
    case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST: return "PatchList";
    default: return "invalid";
    }
}

std::string ToString(VkPolygonMode value) {
    switch (value) {
    case VK_POLYGON_MODE_FILL: return "Fill";
    case VK_POLYGON_MODE_LINE: return "Line";
    case VK_POLYGON_MODE_POINT: return "Point";
    case VK_POLYGON_MODE_FILL_RECTANGLE_NV: return "FillRectangleNV";
    default: return "invalid";
    }
}

std::string ToString(VkCullModeFlagBits value) {
    switch (value) {
    case VK_CULL_MODE_NONE: return "None";
    case VK_CULL_MODE_FRONT_BIT: return "Front";
    case VK_CULL_MODE_BACK_BIT: return "Back";
    case VK_CULL_MODE_FRONT_AND_BACK: return "FrontAndBack";
    default: return "invalid";
    }
}

std::string ToString(bool value) {
    if (value) {
        return "True";
    }
    else {
        return "False";
    }
}


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}
}; // renderer vlk