#pragma once
#include <vector>
#include <optional>
#include "types.h"
#include "vulkan_shared.h"

namespace renderer::vlk {
// Queues are not created explicitly and are not created in this class.
// They are created together with and owned by logical device.
// This class selects queue family indices and provides queue creation strcture 
// to logical device.
// It also stores queue handles that are retrieved from logical device AFTER creation
// of this class. 
// Because order is this - select families - create device - retrieve queue handles
class DeviceQueue {
public:
    // Required device queue family types and their existance
    struct FamilyIndices {
        std::optional<QueueFamilyIndex> graphics;
        std::optional<QueueFamilyIndex> present;
        bool IsComplete() const {
            return graphics.has_value() && present.has_value();
        }
        // If grphics and present is the same queueu
        bool IsGraphicsPresentMatch() const {
            return graphics.value() == present.value();
        }
    };
    DeviceQueue(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface);
    // Get structure necessery for logical device creation
    std::vector<VkDeviceQueueCreateInfo> GetCreateInfos() const;
    const FamilyIndices& GetFamilyIndices() const;
    void SetGraphics(VkQueue queue);
    void SetPresent(VkQueue queue);
    const VkQueue& GetGraphics() const;
    const VkQueue& GetPresent() const;
    // Submit of given command buffer to given queue 
    // All other submit functions call this function
    void Submit(const VkQueue& queue, const std::vector<VkCommandBuffer>& command_buffers,
        const std::vector<VkSemaphore>& wait_semaphores, const std::vector<VkPipelineStageFlags>& wait_stages,
        const std::vector<VkSemaphore>& signal_semaphores, const VkFence& fence) const;
    // Simplified submit of command buffer without synchronization
    void Submit(const VkQueue& queue, const VkCommandBuffer& command_buffer) const;
    // Submit specific for rendering
    // Submit command buffer to graphics queue
    void SubmitGraphics(const VkCommandBuffer& command_buffer, const VkSemaphore& wait_semaphore,
        const VkSemaphore& signal_semaphore, const VkFence& fence) const;
    // Present image to swapchain
    VkResult Present(const VkSwapchainKHR& swapchain, uint32_t image_index, const VkSemaphore& wait_semaphore) const;
private:
    // Find suitable queue families and store their indices
    FamilyIndices SelectFamilies(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) const;
    // Selected queue family indices
    const FamilyIndices family_indices_;
    // Not const because are initialized later
    VkQueue graphics_queue_ = VK_NULL_HANDLE;
    VkQueue present_queue_ = VK_NULL_HANDLE;
};
}; // renderer vlk