#include "instance.h"
#include <GLFW/glfw3.h>
#include <base/log.h>

namespace renderer::vlk {
Instance::Instance(const base::JSONLoader& setting_loader) : 
    required_extentions_(GetRequiredExtensions(validation_)),
    instance_(Create(setting_loader)) {
    // NOTE: for messanger RAII make separate renderable
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

VkInstance Instance::Create(const base::JSONLoader& setting_loader) const {
    VkApplicationInfo  app_info {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    // NOTE: When this c_str is assigned directly to pApplicationName, it is lost. Probably something
    // related to c_str pointer getting invalid for some reason
    const std::string app_name = setting_loader.Get().at("renderer").at("appName").get<std::string>();
    app_info.pApplicationName = app_name.c_str();
    app_info.applicationVersion = setting_loader.Get().at("renderer").at("appVersion").get<uint32_t>();
    const std::string engine_name = setting_loader.Get().at("renderer").at("engineName").get<std::string>();
    app_info.pEngineName = engine_name.c_str();
    app_info.engineVersion = setting_loader.Get().at("renderer").at("engineVersion").get<uint32_t>();
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

    base::Log::Info("Renderer: app name: '", app_info.pApplicationName, "', app version: ", app_info.applicationVersion,
        ", engine name: '", app_info.pEngineName, "', engine version: ", app_info.engineVersion);
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