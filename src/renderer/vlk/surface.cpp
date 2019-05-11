#include "surface.h"
#include <stdexcept>
#include <util/log.h>

namespace renderer::vlk {
Surface::Surface(const VkInstance& instance, GLFWwindow* window) : 
    instance_(instance),
    surface_(Create(instance_, window)) {
    util::Log::Info("Renderer: surface created");
}

Surface::~Surface() {
    util::Log::Info("Renderer: surface destroying...");
    vkDestroySurfaceKHR(instance_, surface_, nullptr);
}

const VkSurfaceKHR& Surface::Get() const {
    return surface_;
}

VkSurfaceKHR Surface::Create(const VkInstance& instance, GLFWwindow* window) const {
    VkSurfaceKHR surface;
    // No typical error check here because this is GLFW function
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Renderer: failed to create window surface");
    }
    return surface;
}
}; // renderer vlk