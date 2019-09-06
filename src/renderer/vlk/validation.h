#pragma once
#include "vulkan_shared.h"
#include "types.h"

namespace renderer::vlk {
// Validation layers and debug messanger and callback
// TODO: look into instance->createDebugReportCallbackEXTUnique 
// for maybe simpler use instead of messanger
class Validation {
public:
    Validation();
    ~Validation();
    bool IsEnabled() const;
    const LayerVector& GetLayers() const;
    // Add required extentions for our validation layers to given extentions vector
    void AppendExtentions(ExtentionVector& extensions) const;
    // Create messanger from instance that will report back from layers
    // NOTE: we can't use RAII here because validation renderable is created
    // before instance (we need to check layer availability first)
    // but messanger needs to be created after instance and destroyed before instance is
    // destroyed.
    // As an alternative add it to instance as another renderable, but this would put validation
    // in two files not one, like in this case.
    // NOTE: Use this method after instance has been initialized
    void CreateMessanger(const VkInstance& instance);
    // NOTE: Call this function before instance is destroyed
    void DestroyMessanger(const VkInstance& instance);
private:
    // Throws if validation layers are not supported
    void CheckSupport() const;
    bool IsLayersSupported() const;
    // For reporting back from validation layers
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
            VkDebugUtilsMessageTypeFlagsEXT message_type,
            const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
            void* user_data);

#ifdef NDEBUG
    static constexpr bool enabled_ = false;
#else
    static constexpr bool enabled_ = true;
#endif
    const LayerVector layers_;
    VkDebugUtilsMessengerEXT debug_messanger_ = VK_NULL_HANDLE;
};
}; // renderer vlk