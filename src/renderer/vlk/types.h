#pragma once
#include <vector>
//#include <string>
#include <vulkan/vulkan.h>

// Graphics vulkan commonly used types
namespace renderer::vlk {
// Types
using ExtentionVector = std::vector<const char*>;
using LayerVector = std::vector<const char*>;
using BufferSize = VkDeviceSize;
using QueueFamilyIndex = uint32_t;
// Structures
/*
// Settings for the renderer
struct Settings {
    std::string app_name;
    uint32_t app_version;
    std::string engine_name;
    uint32_t engine_version;
    // Number of frames that are processed in paralel
    uint32_t frames_in_flight;
};
*/
};
