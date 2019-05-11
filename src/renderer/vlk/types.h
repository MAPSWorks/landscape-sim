#pragma once
#include <vector>
#include <string>

// Graphics vulkan commonly used types
namespace renderer::vlk {
// Types
using ExtentionVector = std::vector<const char*>;
using LayerVector = std::vector<const char*>;
// Structures
// Settings for the renderer
struct Settings {
    std::string app_name;
    uint32_t app_version;
    std::string engine_name;
    uint32_t engine_version;
};
};
