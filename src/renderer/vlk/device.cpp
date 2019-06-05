#include "device.h"
#include <vector>
#include <stdexcept>
#include <base/log.h>

namespace renderer::vlk {
Device::Device(const VkInstance& instance, const VkSurfaceKHR& surface) : 
                required_extentions_({ VK_KHR_SWAPCHAIN_EXTENSION_NAME }),
                gpu_(AcquirePhysicalDevice(instance)),
                queue_(gpu_, surface),
                device_(CreateLogicalDevice(gpu_)) {
    // Retrieve queues from device and set their handles in DeviceQueue class
    queue_.SetGraphics(GetGraphicsQueue());
    queue_.SetPresent(GetPresentQueue());
    base::Log::Info("Renderer: device created");
}

Device::~Device() {
    base::Log::Info("Renderer: device destroying...");
    // Also destroys logical device and queues created from logical device
    vkDestroyDevice(device_, nullptr);
}

const VkPhysicalDevice& Device::GetGPU() const {
    return gpu_;
}

// TODO: for performance should store this variable once and later just return it here
const VkPhysicalDeviceMemoryProperties Device::GetGPUMemoryProperties() const {
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(gpu_, &mem_properties);
    return mem_properties;
}

const VkDevice& Device::Get() const {
    return device_;
}

// Get queue abstraction instead of actual queue
const DeviceQueue& Device::GetQueue() const {
    return queue_;
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

void Device::PrintPhysicalDeviceProperties(const VkPhysicalDevice& gpu) const {
    // Log some properties
    VkPhysicalDeviceProperties p_device_properties;
    vkGetPhysicalDeviceProperties(gpu, &p_device_properties);
    base::Log::Info("Renderer: GPU picked - ", p_device_properties.deviceName,
                    ", Vulkan vers. - ", VK_VERSION_MAJOR(p_device_properties.apiVersion), ".",
                    VK_VERSION_MINOR(p_device_properties.apiVersion), ".",
                    VK_VERSION_PATCH(p_device_properties.apiVersion));
}

// Create logical device from given physical device
VkDevice Device::CreateLogicalDevice(const VkPhysicalDevice& gpu) const {
    const auto queue_create_infos = queue_.GetCreateInfos();
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
    ErrorCheck(vkCreateDevice(gpu, &device_create_info, nullptr, &device));
    return device;
}

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
}; //renderer vlk