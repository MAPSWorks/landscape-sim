#pragma once
#include "vulkan_shared.h"
#include "types.h"

namespace renderer::vlk {
class CommandPool {
public:
    // is_transient - true for short-lived buffers for optimization
    CommandPool(const VkDevice& device, QueueFamilyIndex family_index, bool is_resetable, bool is_transient=false);
    ~CommandPool();
    CommandPool(CommandPool const&) = delete;
    CommandPool operator=(CommandPool const&) = delete;
    const VkCommandPool& Get() const;
    VkCommandBuffer AllocateCommandPrimaryBuffer() const;
private:
    VkCommandPool Create(QueueFamilyIndex family_index, bool is_resetable, bool is_transient) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    const VkCommandPool command_pool_ = VK_NULL_HANDLE;
};
}; // renderer vlk