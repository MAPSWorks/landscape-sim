#pragma once
#include <vector>
#include <optional>
#include "vulkan_shared.h"

namespace renderer::vlk {
// Queues are not created explicitly and are not in this class.
// They are created together with and owned by logical device.
// This class provides only queue create structure to pass to logical device
// creation and also stores selected queue family indices to be used.
class DeviceQueue {
public:
    // Required device queue family types and their existance
    struct FamilyIndices {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;
        bool IsComplete() const {
            return graphics.has_value() && present.has_value();
        }
        // If grphics and present is the same queueu
        bool IsGraphicsPresentMatch() const {
            return graphics.value() == present.value();
        }
    };
    DeviceQueue(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface);
    // Get structure necessery for logical device creation
    std::vector<VkDeviceQueueCreateInfo> GetCreateInfos(const VkPhysicalDevice& physical_device) const;
    const FamilyIndices& GetFamilyIndices() const;
private:
    // Find suitable queue families and store their indices
    FamilyIndices SelectFamilies(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface) const;
    // Selected queue family indices
    const FamilyIndices family_indices_;
};
}; // renderer vlk