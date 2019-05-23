/*
Ideas:
    + Rename vlk -> gdi
    + Swapchain images and their formats, extent in their own class that is attached to swapchain
*/
#include "context.h"
#include <base/log.h>

namespace renderer {
Context::Context(const vlk::Settings& settings, GLFWwindow* window) :
    instance(settings),
    surface(instance.Get(), window),
    device(instance.Get(), surface.Get()) {
    base::Log::Info("Renderer: context created");
}

Context::~Context() {
    base::Log::Info("Renderer: context destroying...");
}
}; // graphics vlk