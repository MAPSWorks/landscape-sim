#pragma once
#include "vulkan_shared.h"
#include "device_queue.h"
#include "types.h"

namespace renderer::vlk {
// Physical and logical devices with associated features and properties
class Device {
public:
    Device(const VkInstance& instance, const VkSurfaceKHR& surface);
    ~Device();
    Device(Device const&) = delete;
    Device operator=(Device const&) = delete;
    const VkPhysicalDevice& GetGPU() const;
    const VkDevice& Get() const;
    // Get queue abstraction instead of actual queue
    const DeviceQueue& GetDeviceQueue() const;
    VkQueue GetGraphicsQueue() const;
    VkQueue GetPresentQueue() const;
private:
    // Select propper physical device
    VkPhysicalDevice AcquirePhysicalDevice(const VkInstance& instance) const;
    void PrintPhysicalDeviceProperties(const VkPhysicalDevice& physical_device) const;
    VkDevice CreateLogicalDevice(const VkPhysicalDevice& physical_device) const;
    // Desired device extentions
    const ExtentionVector required_extentions_;
    // The selected device Vulkan is going to use, aka physical device
    const VkPhysicalDevice gpu_ = VK_NULL_HANDLE;
    // Since queues are logically tied to device, they belong here.
    // Actual queues are retrieved from device itself, this object stores families indices
    // and provides additional helper functions
    const DeviceQueue queue_;
    // Logical device can be thought of as physical device with enabled certain features
    const VkDevice device_ = VK_NULL_HANDLE;
};
}; // renderer vlk