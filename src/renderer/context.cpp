/*
Ideas:
    + Rename vlk -> gdi
    + Swapchain images and their formats, extent in their own class that is attached to swapchain
*/
#include "context.h"
#include <util/log.h>

namespace renderer {
Context::Context(const vlk::Settings& settings, GLFWwindow* window) :
    instance(settings),
    surface(instance.Get(), window),
    device(instance.Get(), surface.Get()),
    swapchain(device, surface.Get(), window) {
    util::Log::Info("Renderer: context initialized");
}

Context::~Context() {
    util::Log::Info("Renderer: context shutting down...");
}
}; // graphics vlk