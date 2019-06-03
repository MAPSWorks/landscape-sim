#pragma once
#include "vulkan_shared.h"

namespace renderer::vlk {
class CommandPool {
public:
    CommandPool(const VkDevice& device, uint32_t family_index, bool is_resetable);
    ~CommandPool();
    CommandPool(CommandPool const&) = delete;
    CommandPool operator=(CommandPool const&) = delete;
    VkCommandBuffer AllocateCommandPrimaryBuffer() const;
private:
    VkCommandPool Create(uint32_t family_index, bool is_resetable) const;
    // Reference to resource this object is created with
    const VkDevice& device_;
    const VkCommandPool command_pool_ = VK_NULL_HANDLE;
};
}; // renderer vlk