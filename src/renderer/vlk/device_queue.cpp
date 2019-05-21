#include "device_queue.h"
#include <set>
#include <stdexcept>
#include <base/log.h>

namespace renderer::vlk {
DeviceQueue::DeviceQueue(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface) :
                                                family_indices_(SelectFamilies(gpu, surface)) {
    base::Log::Info("Renderer: device queue family indices picked. Graphics - ", 
                                               family_indices_.graphics.value(), 
                                " present - ", family_indices_.present.value());
}

std::vector<VkDeviceQueueCreateInfo> DeviceQueue::GetCreateInfos() const{
    // We dont know in advance whether queue family capabilities belong to one family or multiple
    const std::set<uint32_t> unique_queue_families = { family_indices_.graphics.value(), 
                                                       family_indices_.present.value() };
    // Create multiple queues if necessery
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    // Priority for now
    uint32_t number_of_queues = 1;
    float queue_priority = 1.0f;
    for (auto queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = number_of_queues;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }
    return queue_create_infos;
}

const DeviceQueue::FamilyIndices& DeviceQueue::GetFamilyIndices() const {
    return family_indices_;
}

void DeviceQueue::SetGraphics(VkQueue queue) {
    graphics_queue_ = queue;
}

void DeviceQueue::SetPresent(VkQueue queue) {
    present_queue_ = queue;
}

const VkQueue& DeviceQueue::GetGraphics() const {
    return graphics_queue_;
}

const VkQueue& DeviceQueue::GetPresent() const {
    return present_queue_;
}

// Single submit of command buffer to graphics queue
// Performance is important since this is probably real-time function
// wait_semaphore - to wait for executing on pipeline stage
// signal_semaphore - to signal when done
void DeviceQueue::GraphicsSubmit(const VkCommandBuffer& command_buffer, const VkSemaphore& wait_semaphore, 
    const VkSemaphore& signal_semaphore) const {
    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    const VkSemaphore wait_semaphores[] = { wait_semaphore };
    // This means that waiting will happen on color output stage, so vertex and fragments 
    // can be executed meanwhile
    const VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;
    const VkSemaphore signal_semaphores[] = { signal_semaphore };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;
    ErrorCheck(vkQueueSubmit(graphics_queue_, 1, &submit_info, VK_NULL_HANDLE));
}

// Presents iamege to swapchain
// Performance is important since this is probably real-time function
// wait_semaphore - emaphore to wait before presenting
void DeviceQueue::Present(const VkSwapchainKHR& swapchain, uint32_t image_index, const VkSemaphore& wait_semaphore) const {
    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    const VkSemaphore wait_semaphores[] = { wait_semaphore };
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = wait_semaphores;
    VkSwapchainKHR swap_chains[] = { swapchain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = nullptr; // Optional 
    // Error in this doesnt necesserely means that we need to throw exception
    vkQueuePresentKHR(present_queue_, &present_info);
}

DeviceQueue::FamilyIndices DeviceQueue::SelectFamilies(const VkPhysicalDevice& gpu,
                                                       const VkSurfaceKHR& surface) const {
    // Get all available queue families from given physical device
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, queue_families.data());
    FamilyIndices indices;
    uint32_t i = 0;
    for (const auto& family : queue_families) {
        if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = i;
        }
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &present_support);
        if (family.queueCount > 0 && present_support) {
            indices.present = i;
        }
        if (indices.IsComplete()) {
            break;
        }

        i++;
    }
    if (!indices.IsComplete()) {
        throw std::runtime_error("Renderer: failed to find required device queue families");
    }
    return indices;
}

}; // renderer vlk