#include "graphics_system.h"
#include <util/log.h>

namespace graphics {
GraphicsSystem::GraphicsSystem(const Settings& settings, GLFWwindow* window) : 
    context_(settings.renderer_settings, window),
    triangle_(context_) {
    util::Log::Info("Graphics: system initialized");
}

GraphicsSystem::~GraphicsSystem() {
    util::Log::Info("Graphics: system shutting down...");
}

void GraphicsSystem::RenderFrame() const {
    triangle_.Draw();
}
}; // graphics