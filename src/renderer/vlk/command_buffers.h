#pragma once
#include <vector>
#include "vulkan_shared.h"

namespace renderer::vlk {
//Command pool together with command buffer for each framebuffer
// Currently would work for graphics command queues
class CommandBuffers {
public:
    CommandBuffers(const VkDevice& device, uint32_t queue_family_index, uint32_t count);
    ~CommandBuffers();
    CommandBuffers(CommandBuffers const&) = delete;
    CommandBuffers operator=(CommandBuffers const&) = delete;
    const std::vector<VkCommandBuffer>& Get() const;
    const VkCommandBuffer& Get(uint32_t index) const;
    // Recording command buffers
    // Inddividual recording
    void Begin(uint32_t index) const;
    void End(uint32_t index) const;
    void BeginRenderPass(uint32_t index, const VkRenderPass& render_pass, const std::vector<VkFramebuffer>& frame_buffers,
        const VkExtent2D& render_area_extent) const;
    void EndRenderPass(uint32_t index) const;
    void BindGraphicsPipeline(uint32_t index, const VkPipeline& pipeline) const;
    void Draw(uint32_t index, uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance) const;

private:
    VkCommandPool CreateCommandPool(uint32_t family_index) const;
    std::vector<VkCommandBuffer> AllocateCommandBuffers(uint32_t count) const;
    // Reference to resource this object is created with
    const VkDevice& device_;
    const VkCommandPool command_pool_ = VK_NULL_HANDLE;
    // Command buffers will be created for each framebuffer
    const std::vector<VkCommandBuffer> command_buffers_;
};
}; // renderer vlk