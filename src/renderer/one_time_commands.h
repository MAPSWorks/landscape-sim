#pragma once
#include "vlk/vulkan_shared.h"
#include "vlk/device.h"
#include "vlk/command_pool.h"

// Helper class to execute commands that require on-time submit
// command buffers. Stores apropriate command pool for that kind of buffers
// and eases this submission process.
// Must be used as single instance - multiple uses.
namespace renderer {
class OneTimeCommands {
public:
    OneTimeCommands(const vlk::Device& device);
    // Copies contents of one buffer to other
    void CopyBuffer(const VkBuffer& src_buffer, const VkBuffer& dst_buffer, vlk::BufferSize size) const;
private:
    // Reference to resource this renderable is created with
    const vlk::Device& device_;
    // Command pool for short-lived command buffers..
    // Driver should be able to optimaze if we use separate pool for these transient operation.
    const vlk::CommandPool command_pool_;
};
}; //renderer