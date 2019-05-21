#include "instance.h"
#include <GLFW/glfw3.h>
#include <base/log.h>

namespace renderer::vlk {
Instance::Instance(const Settings& settings) : required_extentions_(GetRequiredExtensions(validation_)),
                                               instance_(Create(settings)) {
    // NOTE: for messanger RAII make separate object
    validation_.CreateMessanger(instance_);
    base::Log::Info("Renderer: instance created");
}

Instance::~Instance() {
    base::Log::Info("Renderer: instance destroyting...");
    validation_.DestroyMessanger(instance_);
    vkDestroyInstance(instance_, nullptr);
}

const VkInstance& Instance:: Get() const {
    return instance_;
}

VkInstance Instance::Create(const Settings& settings) const {
    VkApplicationInfo  app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = settings.app_name.c_str();
    app_info.applicationVersion = settings.app_version;
    app_info.pEngineName = settings.engine_name.c_str();
    app_info.engineVersion = settings.engine_version;
    app_info.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo  create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    // Extentions
    create_info.enabledExtensionCount = static_cast<uint32_t>(required_extentions_.size());
    create_info.ppEnabledExtensionNames = required_extentions_.data();
    // Validation layers
    if (validation_.IsEnabled()) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_.GetLayers().size());
        create_info.ppEnabledLayerNames = validation_.GetLayers().data();
    }
    else {
        create_info.enabledLayerCount = 0;
    }
    VkInstance instance;
    ErrorCheck(vkCreateInstance(&create_info, nullptr, &instance));
    return instance;
}

// Gather required instance extentions
 ExtentionVector Instance::GetRequiredExtensions(const Validation& validation) const {
    uint32_t glfw_extention_count = 0;
    auto glfw_extentions = glfwGetRequiredInstanceExtensions(&glfw_extention_count);
    ExtentionVector extensions(glfw_extentions, glfw_extentions + glfw_extention_count);
    // Add extentions needed for validation
    validation.AppendExtentions(extensions);
    return extensions;
}
}; // renderer vlk