#include "device_queue.h"
#include <set>
#include <stdexcept>
#include <util/log.h>

namespace renderer::vlk {
DeviceQueue::DeviceQueue(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface) :
                                                family_indices_(SelectFamilies(physical_device, surface)) {
    util::Log::Info("Renderer: device queue family indices picked. Graphics - ", 
                                               family_indices_.graphics.value(), 
                                " present - ", family_indices_.present.value());
}

std::vector<VkDeviceQueueCreateInfo> DeviceQueue::GetCreateInfos(const VkPhysicalDevice& physical_device) const{
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

DeviceQueue::FamilyIndices DeviceQueue::SelectFamilies(const VkPhysicalDevice& physical_device,
                                                       const VkSurfaceKHR& surface) const {
    // Get all available queue families from given physical device
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    FamilyIndices indices;
    uint32_t i = 0;
    for (const auto& family : queue_families) {
        if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics = i;
        }
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);
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