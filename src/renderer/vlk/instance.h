#pragma once
#include "vulkan_shared.h"
#include "types.h"
#include "validation.h"

namespace renderer::vlk {
// Instance stores all aplication-wide vulkan state
class Instance {
public:
    Instance(const Settings& settings);
    ~Instance();
    Instance(Instance const&) = delete;
    Instance operator=(Instance const&) = delete;
    const VkInstance& Get() const;
private:
    VkInstance Create(const Settings& settings) const;
    ExtentionVector GetRequiredExtensions(const Validation& validation) const;
    // Validation layers
    Validation validation_;
    // Required instance extentions
    const ExtentionVector required_extentions_;
    const VkInstance instance_ = VK_NULL_HANDLE;
};
}; // renderer vlk