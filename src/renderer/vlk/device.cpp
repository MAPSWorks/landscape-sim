#include "device.h"
#include <vector>
#include <stdexcept>
#include <util/log.h>

namespace renderer::vlk {
Device::Device(const VkInstance& instance, const VkSurfaceKHR& surface) : 
                required_extentions_({ VK_KHR_SWAPCHAIN_EXTENSION_NAME }),
                gpu_(AcquirePhysicalDevice(instance)),
                queue_(gpu_, surface),
                device_(CreateLogicalDevice(gpu_)) {
    util::Log::Info("Renderer: device created");
}

Device::~Device() {
    util::Log::Info("Renderer: device destroying...");
    vkDestroyDevice(device_, nullptr);
}

const VkPhysicalDevice& Device::GetGPU() const {
    return gpu_;
}

const VkDevice& Device::Get() const {
    return device_;
}

// Get queue abstraction instead of actual queue
const DeviceQueue& Device::GetDeviceQueue() const {
    return queue_;
}

// Queues are taken directly from device and not stored anywhere else
// OPTI: store queue handles in Queue object and set them like this in device c-tor
// and these functions below would not be needed any more, queues would be directly accessed from 
// Queue class through references
VkQueue Device::GetGraphicsQueue() const {
    VkQueue queue;
    vkGetDeviceQueue(device_, queue_.GetFamilyIndices().graphics.value(), 0, &queue);
    return queue;
}

VkQueue Device::GetPresentQueue() const {
    VkQueue queue;
    vkGetDeviceQueue(device_, queue_.GetFamilyIndices().present.value(), 0, &queue);
    return queue;
}

// Physical device is not created but acquired and need not be deleted
VkPhysicalDevice Device::AcquirePhysicalDevice(const VkInstance& instance) const {
    // List of all physical devices available
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
    if (!device_count) {
        throw std::runtime_error("Renderer: failed to find GPU with Vulkan support");
    }
    std::vector<VkPhysicalDevice> physical_devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data());
    // TODO: pick physical device from the list here
    // For now pick first one in the list
    VkPhysicalDevice physical_device = physical_devices.at(0);
    PrintPhysicalDeviceProperties(physical_device);
    return physical_device;
}

void Device::PrintPhysicalDeviceProperties(const VkPhysicalDevice& physical_device) const {
    // Log some properties
    VkPhysicalDeviceProperties p_device_properties;
    vkGetPhysicalDeviceProperties(physical_device, &p_device_properties);
    util::Log::Info("Renderer: GPU picked - ", p_device_properties.deviceName,
                    ", Vulkan vers. - ", VK_VERSION_MAJOR(p_device_properties.apiVersion), ".",
                    VK_VERSION_MINOR(p_device_properties.apiVersion), ".",
                    VK_VERSION_PATCH(p_device_properties.apiVersion));
}

VkDevice Device::CreateLogicalDevice(const VkPhysicalDevice& physical_device) const {
    const auto queue_create_infos = queue_.GetCreateInfos(physical_device);
    // Device features to enable
    VkPhysicalDeviceFeatures device_features {};
    VkDeviceCreateInfo device_create_info {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = queue_create_infos.data();
    device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = static_cast<uint32_t>(required_extentions_.size());
    device_create_info.ppEnabledExtensionNames = required_extentions_.data();

    VkDevice device;
    ErrorCheck(vkCreateDevice(physical_device, &device_create_info, nullptr, &device));
    return device;
}
}; //renderer vlk