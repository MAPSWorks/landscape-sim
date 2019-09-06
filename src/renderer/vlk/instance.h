#pragma once
#include <base/json_loader.h>
#include "vulkan_shared.h"
#include "types.h"
#include "validation.h"

namespace renderer::vlk {
// Instance stores all aplication-wide vulkan state
class Instance {
public:
    Instance(const base::JSONLoader& setting_loader);
    ~Instance();
    Instance(Instance const&) = delete;
    Instance operator=(Instance const&) = delete;
    const VkInstance& Get() const;
private:
    VkInstance Create(const base::JSONLoader& setting_loader) const;
    ExtentionVector GetRequiredExtensions(const Validation& validation) const;
    // Validation layers
    Validation validation_;
    // Required instance extentions
    const ExtentionVector required_extentions_;
    const VkInstance instance_ = VK_NULL_HANDLE;
};
}; // renderer vlk