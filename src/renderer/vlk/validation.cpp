#include "validation.h"
#include <stdexcept>
#include <util/log.h>

namespace renderer::vlk {
// Prior to Vulkan SDK for 1.1.106.0 layers: VK_LAYER_LUNARG_standard_validation
// Starting from Vulkan SDK for 1.1.106.0 : VK_LAYER_KHRONOS_validation
Validation::Validation() : layers_({ "VK_LAYER_KHRONOS_validation" }) {
    if (enabled_) {
        CheckSupport();
        util::Log::Info("Renderer: validation initialized");
    }
}

Validation::~Validation() {
    if (enabled_) {
        util::Log::Info("Renderer: validation shutting down...");
    }
}

bool Validation::IsEnabled() const {
    return enabled_;
}

const LayerVector& Validation::GetLayers() const {
    return layers_;
}

// Add required extentions for our validation layers to given extentions vector
void Validation::AppendExtentions(ExtentionVector& extensions) const {
    if (enabled_) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // same as 'VK_EXT_debug_report'
    }
}

// Create messanger from instance that will report back from layers
// NOTE: we can't use RAII here because validation object is created
// before instance (we need to check layer availability first)
// but messanger needs to be created after instance and destroyed before instance is
// destroyed.
// NOTE: Use this method after instance has been initialized
void Validation::CreateMessanger(const VkInstance& instance) {
    if (enabled_) {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = DebugCallback;
        create_info.pUserData = nullptr; // Optional (goes to user_data in debug callback)
        ErrorCheck(CreateDebugUtilsMessengerEXT(instance, &create_info, nullptr, &debug_messanger_));
    }
}

// NOTE: Call this function before instance is destroyed
void Validation::DestroyMessanger(const VkInstance& instance) {
    if (enabled_) {
        DestroyDebugUtilsMessengerEXT(instance,debug_messanger_, nullptr);
    }
}

void Validation::CheckSupport() const {
    if (enabled_ && !IsLayersSupported()) {
        throw std::runtime_error("Renderer: validation layers are not available!");
    }
}

// Check if required layers are available
bool Validation::IsLayersSupported() const {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
    for (auto name_ : layers_) {
        bool found_ = false;
        for (const auto& properties_ : available_layers) {
            if (strcmp(name_, properties_.layerName) == 0) {
                found_ = true;
                break;
            }
        }
        if (!found_) {
            return false;
        }
    }

    return true;
}

// Reports back errors from layer
VKAPI_ATTR VkBool32 VKAPI_CALL Validation::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
                                                            VkDebugUtilsMessageTypeFlagsEXT message_type, 
                                                            const VkDebugUtilsMessengerCallbackDataEXT* callback_data, 
                                                            void* user_data) {
    util::Log::Error("* Validation layer: ", callback_data->pMessage);
    return VK_FALSE;
}
}; // renderer vlk