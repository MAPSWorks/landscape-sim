#pragma once
#include "vulkan_shared.h"
#include "command_pool.h"

// Command buffer is not created explicitly, but allocated from command pool
// It is also implicitly destroyed when command pool that created it is destroyed
namespace renderer::vlk {
class CommandBuffer {
public:
    CommandBuffer(const CommandPool& command_pool);
    const VkCommandBuffer& Get() const;
    // Command beffer recording
    void Begin() const;
    void End() const;
    void BeginRenderPass(const VkRenderPass& render_pass, const VkFramebuffer& frame_buffer,
        const VkExtent2D& render_area_extent) const;
    void EndRenderPass() const;
    void BindGraphicsPipeline(const VkPipeline& pipeline) const;
    void BindBuffer(const VkBuffer& buffer, uint32_t binding_index = 0) const;
    void Draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const;

private:
    VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;;
};
}; // renderer vlk