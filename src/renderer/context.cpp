/*
Ideas:
    + Rename vlk -> gdi
    + Swapchain images and their formats, extent in their own class that is attached to swapchain
*/
#include "context.h"
#include <base/log.h>

namespace renderer {
Context::Context(const base::JSONLoader& setting_loader, GLFWwindow* window) :
    window_glfw(window),
    instance(setting_loader),
    surface(instance.Get(), window_glfw),
    device(instance.Get(), surface.Get()) {
    base::Log::Info("Renderer: context created");
}
}; // graphics vlk